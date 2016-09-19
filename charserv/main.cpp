/* timeserv.c */
/* A simple UDP server that sends the current date and time to the client */
/* Last modified: September 20, 2005 */
/* http://www.gomorgan89.com */
/* Link with library file wsock32.lib */

#include "charserv.h"


#include "netlib_server.h"

#include "Game.h"
#include "draw.h"

#include "GCore_function_macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>

#include <windows.h>


GDEFINE_POD(klib, SUserMetadata		);
GDEFINE_OBJ(klib, SUserCredentials	);
GDEFINE_OBJ(klib, SPlayer			);

void usage(void);


void WriteTitle( int ThreadNum )
{
    static const int32_t sizeOfNThreadMsg = 80;

	char    NThreadMsg[sizeOfNThreadMsg];
    sprintf_s( NThreadMsg, sizeOfNThreadMsg, "Threads running: %02d. Ctrl+Q to Quit.", ThreadNum );
    SetConsoleTitleA( NThreadMsg );
}

bool bListenFailure = false;
void serverListen( ktools::CServer* pServer )
{
	while( !bListenFailure )
	{
		if( pServer->Listen() )
		{
			error_print("Failed to listen on server.");
			bListenFailure = true;
		}
		else if( false == bListenFailure && 0 > pServer->Accept() )
		{
			error_print("Failed to accept queued client or no client queued.");
		}
		// Get display screen information & clear the screen.
		Sleep(100);
		WriteTitle( pServer->ClientConnections.size() );
	};
}

void serverListen( void* server )
{
	serverListen((ktools::CServer*)server);
}

// Use this function to draw our game data
void draw( klib::SGame& instanceGame ) // 
{
 	klib::drawAndPresentGame(instanceGame);
};


int main(int argc, char **argv)// Thread 1: main 
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	klib::SPlayerServer serverInstance;

	int port_number;		// Port number to use
	ktools::CServer server;

	// Only run if port is provided
	if (argc == 2)
	{
		if (sscanf_s(argv[1], "%u", &port_number) != 1)
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
	if (ktools::initNetwork())
	{
		error_print("Failed to initialize network.");
		return -1;
	}

	if( 0 > server.InitServer(port_number) )
	{
		error_print("Failed to initialize connection server.");
		return -1;
	};

	// Print out server information
	_beginthread( serverListen, 0, &server );

	debug_print("Press CTRL + Q to quit");

	// Check for exit request while the server is running.
	ktools::initASCIIScreen();

	klib::SGame* pInstancedGame = new klib::SGame;
	klib::SGame& instanceGame = *pInstancedGame;

	klib::initGame(instanceGame);

	debug_printf("sizeof(SCharacter): %u"		, (uint32_t) sizeof(klib::SCharacter));
	debug_printf("sizeof(SPlayerGoods): %u"			, (uint32_t) sizeof(klib::SPlayerGoods));
	debug_printf("sizeof(SPlayer): %u"			, (uint32_t) sizeof(klib::SPlayer));
	debug_printf("sizeof(SPlayerServer): %u"	, (uint32_t) sizeof(klib::SPlayerServer));
	debug_printf("sizeof(SUserCredentials): %u"	, (uint32_t) sizeof(klib::SUserCredentials));
	debug_printf("sizeof(SUserMetadata): %u"	, (uint32_t) sizeof(klib::SUserMetadata));

	while(gbit_true(instanceGame.Flags, klib::GAME_FLAGS_RUNNING) && !bListenFailure)
	{
		instanceGame.ServerTime = time(0);
		pollInput(instanceGame.FrameInput);
		draw(instanceGame);
	}

	if(pInstancedGame)
		delete(pInstancedGame);

	server.ShutdownServer();

	ktools::shutdownNetwork();

	const char serverDBFile [] = "player_server.bin";
	FILE* fp = 0;
	fopen_s(&fp, serverDBFile, "wb");
	if( fp ) 
	{
		for(uint32_t iUser=0, userCount = (uint32_t)serverInstance.Players.size(); iUser<userCount; ++iUser)
		{
			klib::SPlayer* player = serverInstance.Players[iUser];
			if(0 == player) 
				continue;

			//player->
		}
		fclose(fp);
	}

	return 0;
}

void usage(void)
{
	fprintf(stderr, "timeserv [server_address] port\n");
}

int32_t executeCommand(ktools::CClient* client, const char* buffer)
{
	int port_number;	
	int a1, a2, a3, a4;	
	getAddress( client->m_ClientTarget, &a1, &a2, &a3, &a4, &port_number );
	debug_printf("Received charserv command from %u.%u.%u.%u:%u.", 
		(int)a1,
		(int)a2,
		(int)a3,
		(int)a4,
		(int)port_number
	);

	// Get current time
	uint64_t current_time = time(0);

	if (strcmp(buffer, "GET PLAYER\r\n") == 0)
	{
		klib::SPlayer player;
		if(sendUserCommand(client->m_ClientListener, ktools::USER_COMMAND_RESPONSE, (const char*)&current_time, (int)sizeof(current_time), client->m_ClientTarget))
		{
			error_printf("Failed to send player data to %u.%u.%u.%u:%u.", 
				(int)a1,
				(int)a2,
				(int)a3,
				(int)a4,
				(int)port_number
			);
		};

		debug_printf("Sent player data (%s) to %u.%u.%u.%u:%u.", player.Name.c_str(), 
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

		if(sendUserCommand(client->m_ClientListener, ktools::USER_COMMAND_RESPONSE, (const char*)&mypong, sentBytes, client->m_ClientTarget))
		{
			error_printf("Failed to send player data to %u.%u.%u.%u:%u.", 
				(int)a1,
				(int)a2,
				(int)a3,
				(int)a4,
				(int)port_number
			);
		};

		debug_printf("Sent invalid message response to %u.%u.%u.%u:%u.", 
			(int)a1,
			(int)a2,
			(int)a3,
			(int)a4,
			(int)port_number
		);
	}

	return 0;
};
