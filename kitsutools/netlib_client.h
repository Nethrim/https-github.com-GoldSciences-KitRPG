#include "netlib.h"

#ifndef __NETLIB_CLIENT_H__90237423649827469283746928374698273__
#define __NETLIB_CLIENT_H__90237423649827469283746928374698273__


namespace ktools
{

	struct SNetworkClient
	{
		int32_t a1=192, a2=168, a3=1, a4=27;	/* Server address components in xxx.xxx.xxx.xxx form */
		int32_t port_number = 6667;				/* The port number to use */
		int32_t b1=0, b2=0, b3=0, b4=0;			/* Client address components in xxx.xxx.xxx.xxx form */

		SConnectionEndpoint* pServer = 0;	// the remote endpoint
		SConnectionEndpoint* pClient = 0;	// us
	};

	int32_t	initClientConnection	(SNetworkClient& instanceClient);
	int32_t	connect					(SNetworkClient& instanceClient);
	void	disconnectClient		(SNetworkClient& client);
	int		serverTime				(SNetworkClient& instanceClient, time_t& current_time);

} // namespace




#endif // __NETLIB_CLIENT_H__90237423649827469283746928374698273__
