#include "netlib.h"

#define GODS_MTSUPPORT
#include "GCore_mutex.h"
#include "glist.h"

#ifndef __NETLIB_SERVER_H__9823409236498237469827364982734689237__
#define __NETLIB_SERVER_H__9823409236498237469827364982734689237__


#define MAX_CLIENTS_QUEUE 32

#define INTERLOCKED_INCREMENT(n)	InterlockedIncrement(&n)
#define INTERLOCKED_DECREMENT(n)	InterlockedDecrement(&n)
namespace ktools
{
	class CServer;
	class CClient
	{
	public:
		int32_t					m_id				= -1;
		SConnectionEndpoint*	m_ClientListener	= 0;
		SConnectionEndpoint*	m_ClientTarget		= 0;

		int32_t InitClient(SConnectionEndpoint* listener, SConnectionEndpoint* target, int32_t id)
		{
			m_ClientListener	= listener;
			m_ClientTarget		= target;
			m_id = id;
			return 0;
		};
	};
	GDECLARE_OBJ(CClient);

	class CServer
	{
	public:
		volatile long						m_nQueuedClientCount					= 0;
		SConnectionEndpoint*				m_serverConnection						= 0;
		SConnectionEndpoint*				m_QueuedConnections[MAX_CLIENTS_QUEUE]	= {};
		GLstObj(ktools, CClient)			ClientConnections						= {};	
		GLstObj(ktools, CClient)			ClientUnused							= {};	
		god::CGMutex						ConnectionsMutex						= {};
		char								host_name[256]							= {};	//
		bool								m_bListening							= false;	 

		int32_t								InitServer(int32_t port_number);
		int32_t								Listen( void );
		int32_t								Accept( void );
		void								ShutdownServer();
		void								DisconnectClient(int32_t id);

	};
};

// Define this function for processing your commands.
int32_t executeCommand(ktools::CClient* client, const char* buffer);

#endif  // __NETLIB_SERVER_H__9823409236498237469827364982734689237__