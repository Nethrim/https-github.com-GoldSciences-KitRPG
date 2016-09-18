/* timeserv.c */
/* A simple UDP server that sends the current date and time to the client */
/* Last modified: September 20, 2005 */
/* http://www.gomorgan89.com */
/* Link with library file wsock32.lib */
#include "netlib_server.h"

#include "Game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>

#include <windows.h>

#define BUFFER_SIZE 4096

void usage(void);

void WriteTitle( int ThreadNum );    // Display title bar information 

bool bListenFailure = false;
bool bTerminate = false;
void serverListen( void* server )
{
	klib::CServer* pServer = (klib::CServer*)server;
	while( !bListenFailure && !bTerminate )
	{
		if( pServer->Listen() )
		{
			fprintf(stderr, "Failed to listen on server.\n");
			bListenFailure = true;
		}
		else if( ( !bListenFailure && !bTerminate ) && pServer->Accept() )
		{
			fprintf(stderr, "Failed to accept queued client or no client queued.\n");
		}
		// Get display screen information & clear the screen.
		Sleep(100);
		WriteTitle( pServer->ClientConnections.size() );
	};
}

int main(int argc, char **argv)// Thread 1: main 
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	int port_number;		// Port number to use
	int a1, a2, a3, a4;		// Components of address in xxx.xxx.xxx.xxx form
	klib::CServer server;

	/* Interpret command line */
	if (argc == 2)
	{
		/* Use local address */
		if (sscanf_s(argv[1], "%u", &port_number) != 1)
		{
			usage();
			return 0;
		}
	}
	else if (argc == 3)
	{
		/* Copy address */
		if (sscanf_s(argv[1], "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4)
		{
			usage();
			return 0;
		}
		if (sscanf_s(argv[2], "%u", &port_number) != 1)
		{
			usage();
			return 0;
		}
	}
	else
	{
		usage();
		return 0;
	}

    // Get display screen information & clear the screen.
    WriteTitle( 0 );

	// Open windows connection 
	if (initNetwork())
	{
		fprintf(stderr, "Failed to initialize network.\n");
		return -1;
	}

	if( 0 > server.InitServer(port_number) )
	{
		fprintf(stderr, "Failed to initialize connection server.\n");
		return -1;
	};
	// Print out server information

	_beginthread( serverListen, 0, &server );

	printf("Press CTRL + Q to quit\n");

	// Check for exit request while the server is running.
	while (!bTerminate && !bListenFailure)
	{
		if( GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('Q') )
		{
			bTerminate = true;
			//Sleep(10);
		}
	}


	server.ShutdownServer();

	shutdownNetwork();
	return 0;
}

void usage(void)
{
	fprintf(stderr, "timeserv [server_address] port\n");
}


void WriteTitle( int ThreadNum )
{
    static const int32_t sizeOfNThreadMsg = 80;

	char    NThreadMsg[sizeOfNThreadMsg];
    sprintf_s( NThreadMsg, sizeOfNThreadMsg, "Threads running: %02d. Ctrl+Q to Quit.", ThreadNum );
    SetConsoleTitleA( NThreadMsg );
}


int32_t klib::executeCommand(klib::CClient* client, const char* buffer)
{
	// Check for time request 
	if (strcmp(buffer, "GET TIME\r\n") == 0)
	{
		// Get current time
		time_t current_time = time(NULL);
			
		// Send data back
		int32_t sentBytes = 0;
		if( 0 > sendToConnection( client->m_ClientListener, (char *)&current_time, (int)sizeof(current_time), &sentBytes, client->m_ClientTarget ) )
		{
			fprintf(stderr, "Error sending datagram.\n");
			return -1;
		}
		else if( sentBytes != (int)sizeof(current_time) )
		{
			fprintf(stderr, "Error sending datagram.\n");
			return -1;
		}
		// Display time
		char timestring[256];
		ctime_s(timestring, sizeof(char)*256, &current_time);
		printf("Current time: %s", timestring);
		int port_number;			// Port number to use
		int a1, a2, a3, a4;			// Components of address in xxx.xxx.xxx.xxx form
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		fprintf(stderr, "Sent time (%s) to %u.%u.%u.%u:%u.\n", timestring, 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}
	else if (strcmp(buffer, "GET PLAYER\r\n") == 0)
	{
		// Get current time
		time_t current_time = time(NULL);
		
		SPlayer player;

		// Send data back
		int32_t sentBytes = 0;
		if( 0 > sendToConnection( client->m_ClientListener, (char *)&current_time, (int)sizeof(current_time), &sentBytes, client->m_ClientTarget ) )
		{
			fprintf(stderr, "Error sending datagram.\n");
			return -1;
		}
		else if( sentBytes != (int)sizeof(current_time) )
		{
			fprintf(stderr, "Error sending datagram.\n");
			return -1;
		}
		// Display time
		char timestring[256];
		ctime_s(timestring, sizeof(char)*256, &current_time);
		printf("Current time: %s", timestring);
		
		int port_number;	
		int a1, a2, a3, a4;	
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		fprintf(stderr, "Sent player data (%s) to %u.%u.%u.%u:%u.\n", player.Name.c_str(), 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}
	else
	{
		// Get current time
		const char* mypong = "INVALIDMSG\r\n";
			
		// Send data back
		int32_t sentBytes = 0, bytesTosEnd = (int32_t)(sizeof(char)*(strlen(mypong)+1));

		if( 0 > sendToConnection( client->m_ClientListener, mypong, bytesTosEnd, &sentBytes, client->m_ClientTarget ) )
			return -1;
		else if( sentBytes != bytesTosEnd )
		{
			fprintf(stderr, "Error sending datagram.\n");
			return -1;
		}

		int port_number;
		int a1, a2, a3, a4;
		getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
		fprintf(stderr, "Sent invalid message response to %u.%u.%u.%u:%u.\n", 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}

	return 0;
};
