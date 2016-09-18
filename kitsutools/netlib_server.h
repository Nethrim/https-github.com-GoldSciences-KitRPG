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
		int32_t					m_id;
		SConnectionEndpoint*	m_ClientListener;
		SConnectionEndpoint*	m_ClientTarget;

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
		bool								m_bListening; 
		SConnectionEndpoint*				m_serverConnection;
		char								host_name[256];	/* Name of the server */
		GLstObj(ktools, CClient)			ClientConnections;	
		GLstObj(ktools, CClient)			ClientUnused;	
		SConnectionEndpoint*				m_QueuedConnections[MAX_CLIENTS_QUEUE];
		volatile long						m_nQueuedClientCount;

		int32_t								InitServer(int32_t port_number);
		int32_t								Listen( void );
		int32_t								Accept( void );
		void								ShutdownServer();
		void								DisconnectClient(int32_t id);

		god::CGMutex						ConnectionsMutex;
	};

	enum USER_COMMAND
	{	USER_COMMAND_REQUEST
	,	USER_COMMAND_RESPONSE
	};
	int32_t sendUserCommand(CClient* pClient, USER_COMMAND requestOrResponse, const char* buffer, uint32_t bufferSize);
};

// Define this function for processing your commands.
int32_t executeCommand(ktools::CClient* client, const char* buffer);

#endif  // __NETLIB_SERVER_H__9823409236498237469827364982734689237__