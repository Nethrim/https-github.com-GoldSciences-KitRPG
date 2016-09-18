#include "netlib.h"

#include "glabel.h"

#include <stdio.h>
#include <string>
#if ( defined( WIN32 )|| defined( _WIN32 ) )
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#endif



void initSockaddrStruct( int a1, int a2, int a3, int a4, unsigned short port_number, sockaddr_in* out_sockaddr );

struct SConnectionEndpoint
{
	SOCKET sd					= 0;								/* The socket descriptor */
	struct sockaddr_in sockaddr = {};			/* Information about the client */
};

int32_t initNetwork()
{
	WSADATA w;									/* Used to open Windows connection */
	/* Open windows connection */
	if (WSAStartup(0x0101, &w) != 0)
	{
		error_print("Could not open Windows connection.");
		return -1;
	}
	return 0;
}

int32_t shutdownNetwork()
{
	WSACleanup();
	return 0;
};

size_t _sockaddr_in_len = sizeof(struct sockaddr_in);

void initSockaddrStruct( int a1, int a2, int a3, int a4, unsigned short port_number, sockaddr_in* out_sockaddr ) 
{
	/* Clear out server struct */
	memset(out_sockaddr, 0, _sockaddr_in_len);

	/* Set family and port */
	out_sockaddr->sin_family = AF_INET;
	out_sockaddr->sin_port = htons(port_number);

	/* Set server address */
	out_sockaddr->sin_addr.S_un.S_un_b.s_b1 = (unsigned char)a1;
	out_sockaddr->sin_addr.S_un.S_un_b.s_b2 = (unsigned char)a2;
	out_sockaddr->sin_addr.S_un.S_un_b.s_b3 = (unsigned char)a3;
	out_sockaddr->sin_addr.S_un.S_un_b.s_b4 = (unsigned char)a4;
}

int32_t createConnection( int b1, int b2, int b3, int b4, unsigned short port_number, SConnectionEndpoint** out_newClientInfo ) 
{
	SConnectionEndpoint* newClientInfo = new SConnectionEndpoint();
	initSockaddrStruct( b1, b2, b3, b4, port_number, &newClientInfo->sockaddr );
	*out_newClientInfo = newClientInfo;
	return 0;
}

int32_t createConnection( unsigned short port_number, SConnectionEndpoint** out_clientInfo ) 
{
	char host_name[256];
	gethostname(host_name, sizeof(host_name));
	return createConnectionByHostName( host_name, port_number, out_clientInfo );
};

int32_t createConnectionByHostName( char* host_name, unsigned short port_number, SConnectionEndpoint** out_clientInfo ) 
{
	int b1, b2, b3, b4;							/* Client address components in xxx.xxx.xxx.xxx form */

#if defined( _WIN32) || defined(WIN32)
	sockaddr binaryAddress = {AF_UNSPEC,};
	addrinfo myAddrInfoHints = { AI_CANONNAME, PF_INET, SOCK_DGRAM, IPPROTO_UDP, strlen(host_name), host_name, &binaryAddress };
	std::string portString = std::to_string(port_number);

  //--------------------------------
    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function

	addrinfo hints;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

	

	addrinfo* createdAddrInfo = 0;
	int32_t sockErr = getaddrinfo(host_name, portString.c_str(), &hints, &createdAddrInfo);

	// Retrieve each address and print out the hex bytes
	int i = 1;
//    struct sockaddr_in6 *sockaddr_ipv6;

	for(addrinfo* ptr=createdAddrInfo; ptr != NULL ;ptr=ptr->ai_next) {

        debug_printf("getaddrinfo response %d", i++);
        debug_printf("Flags: 0x%x", ptr->ai_flags);
        debug_print("Family: ");
		DWORD ipbufferlength = 46;
		char ipstringbuffer[46];
		wchar_t ipwstringbuffer[46];
		LPSOCKADDR sockaddr_ip;
		sockaddr_in  *sockaddr_ipv4;
		//DWORD dwRetval;
	    INT iRetval;

		switch (ptr->ai_family) {
            case AF_UNSPEC:
                debug_print("Unspecified");
                break;
            case AF_INET:
                debug_print("AF_INET (IPv4)");
                sockaddr_ipv4 = (sockaddr_in *) ptr->ai_addr;
	            ipbufferlength = 46;
 				inet_ntop(AF_INET, ptr->ai_addr, ipstringbuffer, ipbufferlength);
				debug_printf("IPv4 address %s", ipstringbuffer );
				/* Copy address */
				b1		= sockaddr_ipv4->sin_addr.S_un.S_un_b.s_b1
				, b2	= sockaddr_ipv4->sin_addr.S_un.S_un_b.s_b2
				, b3	= sockaddr_ipv4->sin_addr.S_un.S_un_b.s_b3
				, b4	= sockaddr_ipv4->sin_addr.S_un.S_un_b.s_b4
				;

                //printf("\tIPv4 address %s\n", inet_ntoa(sockaddr_ipv4->sin_addr) );
                break;
            case AF_INET6:
                debug_print("AF_INET6 (IPv6)");
                // the InetNtop function is available on Windows Vista and later
                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                // printf("\tIPv6 address %s\n",
                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
                
                // We use WSAAddressToString since it is supported on Windows XP and later
                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
                // The buffer length is changed by each call to WSAAddresstoString
                // So we need to set it for each iteration through the loop for safety
                ipbufferlength = 46;
                iRetval = WSAAddressToStringW(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, ipwstringbuffer, &ipbufferlength );
                if (iRetval) {
                    debug_printf("WSAAddressToString failed with %u", WSAGetLastError() );
				}
                else    
                    wprintf(L"\tIPv6 address %s", ipwstringbuffer);
                break;
            case AF_NETBIOS:
                debug_print("AF_NETBIOS (NetBIOS)");
                break;
            default:
                debug_printf("Other %ld", ptr->ai_family);
                break;
        }
        debug_print("Socket type: ");
        switch (ptr->ai_socktype) {
            case 0:
                debug_print("Unspecified");
                break;
            case SOCK_STREAM:
                debug_print("SOCK_STREAM (stream)");
                break;
            case SOCK_DGRAM:
                debug_print("SOCK_DGRAM (datagram)");
                break;
            case SOCK_RAW:
                debug_print("SOCK_RAW (raw)");
                break;
            case SOCK_RDM:
                debug_print("SOCK_RDM (reliable message datagram)");
                break;
            case SOCK_SEQPACKET:
                debug_print("SOCK_SEQPACKET (pseudo-stream packet)");
                break;
            default:
                debug_printf("Other %ld", ptr->ai_socktype);
                break;
        }
        debug_print("Protocol: ");
        switch (ptr->ai_protocol) {
            case 0:
                debug_print("Unspecified");
                break;
            case IPPROTO_TCP:
                debug_print("IPPROTO_TCP (TCP)");
                break;
            case IPPROTO_UDP:
                debug_print("IPPROTO_UDP (UDP)");
                break;
            default:
                debug_printf("Other %ld", ptr->ai_protocol);
                break;
        }
        debug_printf("Length of this sockaddr: %d", (int32_t)ptr->ai_addrlen);
        debug_printf("Canonical name: %s", ptr->ai_canonname);
    }

	freeaddrinfo(createdAddrInfo);
	return createConnection( b1, b2, b3, b4, port_number, out_clientInfo );
#else
	struct hostent* hp = gethostbyname(host_name);

	/* Check for NULL pointer */
	if (hp == NULL)
	{
		error_printf("Could not get host by name.");
		return -1;
	}	
	/* Assign the address */
	b1 = hp->h_addr_list[0][0];
	b2 = hp->h_addr_list[0][1];
	b3 = hp->h_addr_list[0][2];
	b4 = hp->h_addr_list[0][3];
	return createConnection( b1, b2, b3, b4, port_number, out_clientInfo );
#endif
};

int32_t initConnection( SConnectionEndpoint* connection ) 
{
	/* Open a datagram socket */
	SOCKET sd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sd == INVALID_SOCKET)
	{
		error_print("Could not create socket.");
		return -1;
	}
	connection->sd = sd;
	return 0;
};

int32_t shutdownConnection( SConnectionEndpoint** out_clientInfo )
{
	if(0 == *out_clientInfo) 
		return 0;
	
	if( (*out_clientInfo)->sd ) 
	{
		closesocket((*out_clientInfo)->sd);
		(*out_clientInfo)->sd = 0;
	}
	delete( *out_clientInfo );
	*out_clientInfo = 0;
	return 0;
}


int32_t bindConnection( SConnectionEndpoint* client )
{
	/* Bind local address to socket */
	int sockaddr_in_len = (int32_t)_sockaddr_in_len;
	if (bind(client->sd, (sockaddr *)&client->sockaddr, (int32_t)_sockaddr_in_len) == -1)
	{
		error_printf("Cannot bind address to socket.");
		return -1;
	}
	getsockname(client->sd,(sockaddr *)&client->sockaddr, &sockaddr_in_len);
	return 0;
}

int32_t connectionListen( SConnectionEndpoint* conn )
{
	if (listen(conn->sd, 1) == SOCKET_ERROR) 
	{
		error_printf("listen failed with error: %ld", WSAGetLastError());
		return -1;
	}
	return 0;
};

int32_t connectionAccept( SConnectionEndpoint* conn, SConnectionEndpoint** out_newConn )
{
	//----------------------
	// Accept the connection.
	SConnectionEndpoint* newConnection = 0;
	createConnection( 0, 0, 0, 0, 0, &newConnection );
	if ((newConnection->sd = accept(conn->sd, (struct sockaddr*)&newConnection->sockaddr, NULL)) == INVALID_SOCKET) 
	{
		newConnection->sd = 0;
		error_printf("accept failed with error: %ld", WSAGetLastError());
		shutdownConnection(&newConnection);
		return -1;
	}
	*out_newConn = newConnection;
	debug_printf("Client connected.");
	return 0;
};

int32_t sendToConnection		( SConnectionEndpoint* connection, const char* buffer, uint32_t bytesToSend, int32_t* _bytesSent, SConnectionEndpoint* targetConnection )
{
	/* Tranmsit data to get time */
	if( 0 == connection )
		return -1;
	int server_length = (int32_t)_sockaddr_in_len;
	int32_t bytesSent = -1;
	if((bytesSent = sendto(connection->sd, buffer, bytesToSend, 0, (sockaddr *)&targetConnection->sockaddr, server_length)) == -1)
	{
		error_printf("Error transmitting data.");
		return -1;
	}
	*_bytesSent = bytesSent;
	return 0;
};

int32_t receiveFromConnection	( SConnectionEndpoint* connection, char* buffer, uint32_t bufLen, int32_t* _bytesReceived, SConnectionEndpoint** newConnection )
{
	int server_length = (int32_t)_sockaddr_in_len;
	int32_t bytesReceived =-1;
	if( newConnection )
	{
		SConnectionEndpoint* client;
		createConnection(0,0,0,0,0,&client);
		/* Receive time */
		if ((bytesReceived = recvfrom(connection->sd, buffer, bufLen, 0, (sockaddr *)&client->sockaddr, &server_length)) < 0)
		{
			shutdownConnection(&client);
			error_printf("Error receiving data.");
			*_bytesReceived = bytesReceived;
			return -1;
		}
		*newConnection = client;
	}
	else
	{
		sockaddr_in server = {};
		if (bytesReceived = recvfrom(connection->sd, buffer, bufLen, 0, (sockaddr *)&server, &server_length) < 0)
		{
			error_printf("Error receiving data.");
			*_bytesReceived = bytesReceived;
			return -1;
		}
	}
	*_bytesReceived = bytesReceived;
	return 0;
};

int32_t getAddress( SConnectionEndpoint* connection, int* a1, int* a2, int* a3, int* a4, int* port_number )
{
	if( a1 ) *a1 = connection->sockaddr.sin_addr.S_un.S_un_b.s_b1;
	if( a2 ) *a2 = connection->sockaddr.sin_addr.S_un.S_un_b.s_b2;
	if( a3 ) *a3 = connection->sockaddr.sin_addr.S_un.S_un_b.s_b3;
	if( a4 ) *a4 = connection->sockaddr.sin_addr.S_un.S_un_b.s_b4;
	if( port_number ) *port_number = connection->sockaddr.sin_port;
	return 0;
};

bool ping(SConnectionEndpoint* pClient, SConnectionEndpoint* pServer)
{
	// send our command
	int32_t bytesTransmitted=-1;
	static const god::glabel strPing = "PING\r\n";
	god::error_t transmResult = sendToConnection( pClient, strPing.c_str(), (int)strPing.size() + 1, &bytesTransmitted, pServer );
	if (transmResult < 0 || bytesTransmitted < 0)//strPing.size())
	{
		error_print("Error transmitting data.");
		return false;
	}

	/* Receive answer */
	bytesTransmitted=-1;
	char buffer[256] = {};
	receiveFromConnection( pClient, buffer, sizeof(buffer), &bytesTransmitted, 0 );
	if( bytesTransmitted < 0 )
	{
		error_print("Error receiving data.");
		return false;
	}
	debug_printf("response: %s", buffer);		

	return true;
}

int32_t initClientConnection(SNetworkClient& instanceClient)
{

	if( createConnection( instanceClient.b1, instanceClient.b2, instanceClient.b3, instanceClient.b4, 0, &instanceClient.pClient ) )
	{
		error_printf("Error creating client connection.");
		return -1;
	}

	if( initConnection(instanceClient.pClient) )
	{
		error_printf("Error initializing client connection.");
	}

	if( bindConnection(instanceClient.pClient) )
	{
		error_printf("Error binding client connection.");
		shutdownConnection(&instanceClient.pClient);
		return -1;
	}

	if( createConnection( instanceClient.a1, instanceClient.a2, instanceClient.a3, instanceClient.a4, instanceClient.port_number, &instanceClient.pServer ) )
	{
		error_printf("Error creating new connection to server.");
		return -1;
	}

	return 0;
}


void disconnectClient(SNetworkClient& client)
{
	/* Get current time */
	debug_print("Disconnecting client.");
	shutdownConnection(&client.pClient);
	shutdownConnection(&client.pServer);
}