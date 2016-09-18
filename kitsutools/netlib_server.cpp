#include "netlib_server.h"
#include "netlib_private.h"

#include "GCore_function_macros.h"

#include <cstdio>
#include <process.h>

#include <time.h>

#include <WinSock2.h>

GDEFINE_OBJ(ktools, CClient);

#define BUFFER_SIZE 4096

using namespace ktools;

int32_t ktools::sendUserCommand(CClient* pClient, USER_COMMAND requestOrResponse, const char* buffer, size_t bufferSize)
{
	// Send data back
	int32_t sentBytes = 0;
	const NETLIB_COMMAND commandUserResponse = (requestOrResponse == USER_COMMAND_REQUEST) ? NETLIB_COMMAND_USER_REQUEST : NETLIB_COMMAND_USER_RESPONSE;
	if( 0 > sendToConnection( pClient->m_ClientListener, (const char*)&commandUserResponse, sizeof(NETLIB_COMMAND), &sentBytes, pClient->m_ClientTarget ) )
	{
		error_print("Error sending datagram.");
		return -1;
	}
	else if( sentBytes != (int)bufferSize )
	{
		error_print("Error sending datagram.");
		return -1;
	}

	sentBytes = 0;
	if( 0 > sendToConnection( pClient->m_ClientListener, buffer, bufferSize, &sentBytes, pClient->m_ClientTarget ) )
	{
		error_print("Error sending datagram.");
		return -1;
	}
	else if( sentBytes != (int)bufferSize )
	{
		error_print("Error sending datagram.");
		return -1;
	}
	// Display time
	int32_t port_number;	
	int a1, a2, a3, a4;	
	getAddress( pClient->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Sent user command of %u bytes to %u.%u.%u.%u:%u."
		, bufferSize
		, (int)a1
		, (int)a2
		, (int)a3
		, (int)a4
		, (int)port_number
	);

	return 0;
};

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
	static const int bufferSize = 256;
	NETLIB_COMMAND command = NETLIB_COMMAND_INVALID;
	receiveFromConnection( m_serverConnection, (char*)&command, sizeof(NETLIB_COMMAND), &bytes_received, &client );
	if (bytes_received < 0)
	{
		error_print("Could not receive datagram.");
		return -1;
	}

	getAddress( client, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Received %u bytes from %u.%u.%u.%u:%u. Command: %s", bytes_received, 
		(int)a1,
		(int)a2,
		(int)a3,
		(int)a4,
		(int)port_number,
		god::genum_definition<NETLIB_COMMAND>::get().get_value_label(command).c_str()
	);

	if(command == NETLIB_COMMAND_CONNECT)
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

int32_t processCommandInternal(CClient* client, NETLIB_COMMAND command)
{
	if (command == NETLIB_COMMAND_DISCONNECT)
	{
		disconnectClient(client);
	}
	else if (command == NETLIB_COMMAND_PING)
	{
		// Pong client
		static const NETLIB_COMMAND send_buffer = NETLIB_COMMAND_PONG;
		int32_t sentBytes = 0;

		if( 0 > sendToConnection( client->m_ClientListener, (const char*)&send_buffer, sizeof(NETLIB_COMMAND), &sentBytes, client->m_ClientTarget ) ) 
		{
			return -1;
		}
		if( sentBytes != sizeof(NETLIB_COMMAND) )
		{
			error_print("Error sending datagram.");
			return -1;
		}

		//
		int port_number;	
		int a1, a2, a3, a4;	
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		debug_printf("Sent %s to %u.%u.%u.%u:%u."
			, god::genum_definition<NETLIB_COMMAND>::get().get_value_label(send_buffer).c_str()
			, (int)a1
			, (int)a2
			, (int)a3
			, (int)a4
			, (int)port_number
			);
	}
	else if (command == NETLIB_COMMAND_TIME_GET)
	{
		static const NETLIB_COMMAND send_buffer = NETLIB_COMMAND_TIME_SET;
		int32_t sentBytes = 0;

		if( 0 > sendToConnection( client->m_ClientListener, (const char*)&send_buffer, sizeof(NETLIB_COMMAND), &sentBytes, client->m_ClientTarget ) ) 
		{
			error_printf("Failed to send command: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(send_buffer));
			return -1;
		}
		if( sentBytes != sizeof(NETLIB_COMMAND) )
		{
			error_printf("Failed to send command: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(send_buffer));
			return -1;
		}

		time_t current_time = time(0);
		sentBytes = 0;
		// Send data back
		if( 0 > sendToConnection( client->m_ClientListener, (char *)&current_time, (int)sizeof(current_time), &sentBytes, client->m_ClientTarget ) )
		{
			error_print("Failed to send time to client.");
			return -1;
		}
		else if( sentBytes != (int)sizeof(current_time) )
		{
			error_print("Failed to send time to client.");
			return -1;
		}

		// Display time
		char timestring[256];
		ctime_s(timestring, sizeof(char)*256, &current_time);
		int port_number;			// Port number to use
		int a1, a2, a3, a4;			// Components of address in xxx.xxx.xxx.xxx form
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		timestring[strlen(timestring)-1] = 0;
		debug_printf("Sent time (%s) to %u.%u.%u.%u:%u.", timestring, 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}
	else if (command == NETLIB_COMMAND_USER_REQUEST)
	{
		char buffer[256] = {};
		return executeCommand(client, buffer);
	}
	else
	{
		static const NETLIB_COMMAND send_buffer = NETLIB_COMMAND_INVALID;
		int32_t sentBytes = 0, bytesToSend = (int32_t)sizeof(send_buffer);

		if( 0 > sendToConnection( client->m_ClientListener, (const char*)&send_buffer, bytesToSend, &sentBytes, client->m_ClientTarget ) ) 
		{
			error_printf("Failed to send command: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(send_buffer));
			return -1;
		}
		if( sentBytes != bytesToSend )
		{
			error_printf("Failed to send command: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(send_buffer));
			return -1;
		}

		int port_number;
		int a1, a2, a3, a4;
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		debug_printf("Sent invalid message response to %u.%u.%u.%u:%u.", 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}
	
	return 0;
}

void clientProc( void *pvClient )
{
	int port_number;			// Port number to use
	int a1, a2, a3, a4;			// Components of address in xxx.xxx.xxx.xxx form 
	CClient* pClient = (CClient*)pvClient;
	char	connID = pClient->m_id;

	do
	{
		// Receive bytes from client
		if( 0 == pClient->m_ClientListener )
		{
			error_printf("Client listener was terminated, exiting thread %u...", pClient->m_id);
			break;
		}

		int32_t bytes_received=0;	
		NETLIB_COMMAND command = NETLIB_COMMAND_INVALID;
		receiveFromConnection( pClient->m_ClientListener, (char*)&command, sizeof(NETLIB_COMMAND), &bytes_received, 0 );
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

		if( processCommandInternal( pClient, command ) )
		{
			error_printf("processCommandInternal failed for command: '%s' (0x%x)(%u).", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(command).c_str(), (uint32_t)command, (uint32_t)command);
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

	int32_t a1, a2, a3, a4, local_port_number, client_port_number;	// Components of address in xxx.xxx.xxx.xxx form
	SConnectionEndpoint* newClientListener = 0, *targetConn = m_QueuedConnections[INTERLOCKED_DECREMENT(m_nQueuedClientCount)];

	getAddress( m_serverConnection, &a1, &a2, &a3, &a4, &local_port_number );
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
		error_print("Failed to bind client listener connection.");
		return -1;
	}
	getAddress( newClientListener, &a1, &a2, &a3, &a4, &local_port_number );
	
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

	// Build listening port message
	NETLIB_COMMAND portCommand = NETLIB_COMMAND_PORT;
			
	// Send data back
	int32_t sentBytes = 0;

	if( 0 > sendToConnection( newClientListener, (const char*)&portCommand, sizeof(NETLIB_COMMAND), &sentBytes, targetConn ) ) //client ) )
		return -1;
	
	if( sentBytes != sizeof(NETLIB_COMMAND) )
	{
		error_print("Failed to send port command to client.");
		return -1;
	}

	local_port_number = htons(local_port_number);
	if( 0 > sendToConnection( newClientListener, (const char*)&local_port_number, sizeof(int32_t), &sentBytes, targetConn ) ) //client ) )
		return -1;
	
	if( sentBytes != sizeof(int32_t) )
	{
		error_print("Failed to send port command to client.");
		return -1;
	}

	// Display time
	getAddress( targetConn, &a1, &a2, &a3, &a4, &client_port_number );
	debug_printf("Sent available port (%u) to %u.%u.%u.%u:%u.", 
		(int)local_port_number,
		(int)a1,
		(int)a2,
		(int)a3,
		(int)a4,
		(int)client_port_number
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
