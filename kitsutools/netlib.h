
#include <cstdint>

#ifndef __NETLIB_H__934892734698236498236498716091782__
#define __NETLIB_H__934892734698236498236498716091782__

struct SConnectionEndpoint;

int32_t initNetwork					( void );
int32_t shutdownNetwork				( void );

int32_t createConnection			( int32_t b1, int32_t b2, int32_t b3, int32_t b4, unsigned short port_number, SConnectionEndpoint** out_newConnection);
int32_t createConnection			( unsigned short port_number, SConnectionEndpoint** out_clientInfo );
int32_t createConnectionByHostName	( char* host_name, unsigned short port_number, SConnectionEndpoint** out_newConnection );

int32_t initConnection				( SConnectionEndpoint*	connection );
int32_t shutdownConnection			( SConnectionEndpoint** connection );
int32_t bindConnection				( SConnectionEndpoint*	connection );

int32_t connectionListen			( SConnectionEndpoint* connection );
int32_t connectionAccept			( SConnectionEndpoint* connection, struct SConnectionEndpoint** newConnection );

int32_t getAddress					( SConnectionEndpoint* connection, int32_t* a1, int32_t* a2, int32_t* a3, int32_t* a4, int32_t* port_number );
int32_t sendToConnection			( SConnectionEndpoint* connection, const char* buffer, uint32_t bytesToSend, int32_t* sentBytes, SConnectionEndpoint* targetConnection );
int32_t receiveFromConnection		( SConnectionEndpoint* connection, char* buffer, uint32_t bufLen, int32_t* receivedBytes, SConnectionEndpoint** newConnection );


bool ping(SConnectionEndpoint* pClient, SConnectionEndpoint* pServer);

struct SNetworkClient
{
	int32_t a1=192, a2=168, a3=1, a4=27;	/* Server address components in xxx.xxx.xxx.xxx form */
	int32_t port_number = 6667;				/* The port number to use */
	int32_t b1=0, b2=0, b3=0, b4=0;			/* Client address components in xxx.xxx.xxx.xxx form */

	SConnectionEndpoint* pServer = 0;	// the remote endpoint
	SConnectionEndpoint* pClient = 0;	// us
};

int32_t initClientConnection(SNetworkClient& instanceClient);

void disconnectClient(SNetworkClient& client);

#endif // __NETLIB_H__934892734698236498236498716091782__
