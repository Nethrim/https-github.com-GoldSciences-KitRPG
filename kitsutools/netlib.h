#include "genum.h"

#ifndef __NETLIB_H__934892734698236498236498716091782__
#define __NETLIB_H__934892734698236498236498716091782__

namespace ktools
{
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
}// namespace


#endif // __NETLIB_H__934892734698236498236498716091782__
