#include "netlib_server.h"
#include "netlib_private.h"

#include "GCore_function_macros.h"

#include <cstdio>
#include <process.h>

#include <WinSock2.h>

GDEFINE_OBJ(ktools, CClient);

#define BUFFER_SIZE 4096

using namespace ktools;

int32_t CServer::InitServer(int32_t port_number)
{
	// Get host name of this computer
	gethostname(host_name, sizeof(host_name));
	if( createConnectionByHostName( host_name, port_number, &m_serverConnection ) )
	{
		error_print("Failed to initialize connection.");
		return -1;
	}

	m_bListening			= false;
	m_nQueuedClientCount	= 0;
	if( initConnection( m_serverConnection ) )
	{
		error_print("Failed to initialize server socket.");
		ShutdownServer();
		return -1;
	};

	if( bindConnection( m_serverConnection ) )
	{
		error_print("Failed to bind server socket.");
		ShutdownServer();
		return -1;
	};
	return 0;
};

int32_t CServer::Listen( void )
{
	int a1, a2, a3, a4, port_number;	/* Components of address in xxx.xxx.xxx.xxx form */
	getAddress( m_serverConnection, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Server running on %u.%u.%u.%u:%u"
		,(unsigned int)a1
		,(unsigned int)a2
		,(unsigned int)a3
		,(unsigned int)a4
		,(unsigned int)ntohs(port_number)
		);
	m_bListening = true;

	// Information about the client
	struct SConnectionEndpoint* client;	

	// Receive bytes from client
	int32_t bytes_received=0;
	char buffer[BUFFER_SIZE];
	receiveFromConnection( m_serverConnection, buffer, BUFFER_SIZE, &bytes_received, &client );
	if (bytes_received < 0)
	{
		error_print("Could not receive datagram.");
		return -1;
	}

	getAddress( client, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Received %u bytes from %u.%u.%u.%u:%u. Message: %s", bytes_received, 
		(int)a1,
		(int)a2,
		(int)a3,
		(int)a4,
		(int)port_number,
		buffer
	);

	if( 0 == strcmp(buffer, "CONNECT\r\n") && m_nQueuedClientCount < MAX_CLIENTS_QUEUE )
		m_QueuedConnections[INTERLOCKED_INCREMENT(m_nQueuedClientCount)-1] = client;
	else
		shutdownConnection(&client);

	return 0;
};


void disconnectClient(CClient* client)
{
	/* Get current time */
	debug_printf("Disconnecting client %u.", client->m_id);
	shutdownConnection(&client->m_ClientListener);
	shutdownConnection(&client->m_ClientTarget);
}

int32_t processCommandInternal(CClient* client, const char* buffer)
{
	if (strcmp(buffer, "PING\r\n") == 0)
	{
		/* Get current time */
		const char* mypong = "PONG\r\n";
			
		/* Send data back */
		int32_t sentBytes = 0, bytesTosEnd = (int32_t)(sizeof(char)*(strlen(mypong)+1));

		if( 0 > sendToConnection( client->m_ClientListener, mypong, bytesTosEnd, &sentBytes, client->m_ClientTarget ) )
			return -1;
		if( sentBytes != bytesTosEnd )
		{
			error_print("Error sending datagram.");
			return -1;
		}
		/* Display time */
		int port_number;			/* Port number to use */
		int a1, a2, a3, a4;					/* Components of address in xxx.xxx.xxx.xxx form */
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		debug_printf("Sent ping response to %u.%u.%u.%u:%u.", 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}
	else if (strcmp(buffer, "DISCONNECT\r\n") == 0)
	{
		disconnectClient(client);
	}
	else
	{
		return executeCommand(client, buffer);
	}
	
	return 0;
}

void clientProc( void *pvClient )
{
	int port_number;			/* Port number to use */
	int a1, a2, a3, a4;			/* Components of address in xxx.xxx.xxx.xxx form */
	CClient* pClient = (CClient*)pvClient;
	char	connID = pClient->m_id;

	do
	{
		char buffer[BUFFER_SIZE]={};

		// Receive bytes from client
		int32_t bytes_received=0;	
		if( 0 == pClient->m_ClientListener )
		{
			error_printf("Client listener was terminated, exiting thread %u...", pClient->m_id);
			break;
		}

		receiveFromConnection( pClient->m_ClientListener, buffer, BUFFER_SIZE, &bytes_received, 0 );
		if (bytes_received < 0)
		{
			error_print("Could not receive datagram.");
			break;
		}

		if( 0 == pClient->m_ClientTarget )
		{
			error_printf("Client target was null. Exiting thread %u...", pClient->m_id);
			break;
		}

		getAddress( pClient->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		debug_printf("Received %u bytes from %u.%u.%u.%u:%u.", bytes_received, 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);

		if( processCommandInternal( pClient, buffer ) )
		{
			error_printf("Unrecognized command: '%s'.", buffer);
		};
    }
    // Repeat while RunMutex is still taken. 
    while ( true );
}

int32_t CServer::Accept( void )
{
	if( 0 == m_nQueuedClientCount )
		return 1;

	static int32_t connectionsAccepted = 0;

	int a1, a2, a3, a4, port_number;	/* Components of address in xxx.xxx.xxx.xxx form */
	SConnectionEndpoint* newClientListener = 0, *targetConn = m_QueuedConnections[INTERLOCKED_DECREMENT(m_nQueuedClientCount)];

	//getAddress( targetConn, &a1, &a2, &a3, &a4, &port_number );
	getAddress( m_serverConnection, &a1, &a2, &a3, &a4, &port_number );
	if( createConnection( a1, a2, a3, a4, 0, &newClientListener ) )
	{
 		error_print("Failed to create client listener.");
		return -1;
	}
	if( 0 > initConnection( newClientListener ) )
	{
		error_print("Failed to initialize client listener connection.");
		return -1;
	}				
	if( 0 > bindConnection( newClientListener ) )
	{
		error_print("Failed to initialize client listener connection.");
		return -1;
	}
	getAddress( newClientListener, &a1, &a2, &a3, &a4, &port_number );
	
	{
		GPtrObj(CClient) newClient;
		bool bFound = false;
		god::CGLock lock(ConnectionsMutex);
		newClient->InitClient( newClientListener, targetConn, connectionsAccepted++ );
		for(uint32_t i=0; i<ClientConnections.size(); ++i)
			if(0 == ClientConnections[i])
			{
				ClientConnections.set(newClient, i);
				bFound = true;
			}

		if(!bFound)
			ClientConnections.push(newClient);

		_beginthread( clientProc, 0, newClient.get_address() );
	}

	/* Build listening port message */
	char mypong[64] = {};
	sprintf_s( mypong, "PORT:%u", (int)htons(port_number) );
			
	/* Send data back */
	int32_t sentBytes = 0, bytesToSend = (int32_t)(sizeof(char)*(strlen(mypong)+1));

	if( 0 > sendToConnection( newClientListener, mypong, bytesToSend, &sentBytes, targetConn ) ) //client ) )
		return -1;
	if( sentBytes != bytesToSend )
	{
		error_print("Error sending datagram.");
		return -1;
	}
	/* Display time */
	getAddress( targetConn, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Sent connect response to %u.%u.%u.%u:%u.", 
		(int)a1,
		(int)a2,
		(int)a3,
		(int)a4,
		(int)port_number
	);

	return 0;
};


void CServer::ShutdownServer()
{
	god::CGLock lockClients(ConnectionsMutex);
	m_bListening = false;

	while ( m_nQueuedClientCount > 0 )
	{
		// Tell thread to die and record its death.
		//ReleaseMutex( hRunMutex ); queued connections don't have any threads
		shutdownConnection(&m_QueuedConnections[INTERLOCKED_DECREMENT(m_nQueuedClientCount)]);
	}

	while ( ClientConnections.size() )
	{
		GPtrNCO(CClient) client;
		ClientConnections.pop(&client);
		if(0 == client)
			continue;

		// Close sockets for this client.
		disconnectClient(client.get_address());
	}
	if( m_serverConnection )
	{
		shutdownConnection(&m_serverConnection);
		m_serverConnection = 0;
	}
}

void CServer::DisconnectClient(int32_t id)
{
	god::CGLock lockClients(ConnectionsMutex);
	for(uint32_t i=0; i<ClientConnections.size(); ++i)
	{
		CClient* client = ClientConnections[i];
		if(client && client->m_id == id) 
		{
			disconnectClient(client);
			GODS(CClient) endClient;
			ClientConnections.pop(&endClient);
			ClientConnections.set(endClient, i);
		}
	}
}
