#include "netlib.h"
#if ( defined( WIN32 )|| defined( _WIN32 ) )
#include <WinSock2.h>
#include <Ws2tcpip.h> 
#endif

#ifndef __NETLIB_PRIVATE_H__92364982734698273469823462384238__
#define __NETLIB_PRIVATE_H__92364982734698273469823462384238__

namespace ktools
{
#pragma pack( push, 1 )
	GDEFINE_ENUM_TYPE (NETLIB_COMMAND, int16_t);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 0, NOOP			);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 1, PING			);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 2, PONG			);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 3, TIME_GET		);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 4, TIME_SET		);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 5, CONNECT		);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 6, PORT			);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 7, DISCONNECT	);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 8, USER_REQUEST	);
	GDEFINE_ENUM_VALUE(NETLIB_COMMAND, 9, USER_RESPONSE	);
#pragma pack(pop)

	struct SConnectionEndpoint
	{
		SOCKET			sd			= 0;	// Socket descriptor
		sockaddr_in		sockaddr	= {};	// Information about this endpoint
	};

	int32_t sendSystemCommand(SConnectionEndpoint* pOrigin, SConnectionEndpoint* pTarget, const NETLIB_COMMAND& commandToSend);
	int32_t receiveSystemCommand(SConnectionEndpoint* pLocal, SConnectionEndpoint* pRemote, NETLIB_COMMAND& commandReceived);

} // namespace

#endif // __NETLIB_PRIVATE_H__92364982734698273469823462384238__