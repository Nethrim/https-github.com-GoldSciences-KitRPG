#define NETLIB_SERVER_MODE_CLIENT
#include "draw.h"
#include "netlib_client.h"
#include "glabel.h"

#include <time.h>
#include <process.h>


#include <crtdbg.h>

// Use this function to draw our game data
void draw( klib::SGame& instanceGame ) // 
{
 	drawAndPresentGame(instanceGame);
};

#define MAX_SEND_SIZE 128

int runCommunications(klib::SGame& instanceGame);
void runCommunications(void* instanceGame)
{
	if(instanceGame)
		runCommunications(*(klib::SGame*)instanceGame);
}

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	int tmp = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF);
	// Clear the upper 16 bits and OR in the desired freqency
	tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;
	// Set the new bits
	_CrtSetDbgFlag(tmp);
#endif

	ktools::initASCIIScreen();
	
	if( ktools::initNetwork() )
	{
		error_printf("Failed to initialize network.");
		return -1;
	}

	klib::SGame* pInstancedGame = new klib::SGame;
	klib::SGame& instanceGame = *pInstancedGame;

	klib::initGame(instanceGame);

	_beginthread(runCommunications, 0, pInstancedGame);

	while(instanceGame.bRunning)
	{
		pollInput(instanceGame.FrameInput);
		draw(instanceGame);
	}

	ktools::shutdownNetwork();

	if(pInstancedGame)
		delete(pInstancedGame);

	return 0;
}


int runCommunications(klib::SGame& instanceGame)
{
	ktools::SNetworkClient instanceClient;
	int32_t bytesTransmitted=-1;
	if(initClientConnection(instanceClient))
	{
		error_print("Failed to connect to server.");
		return -1;
	}

	if(connect(instanceClient))
	{
		error_print("Failed to connect to server.");
		return -1;
	}

	god::error_t result = 0;
	while(instanceGame.bRunning)
	{
		// Ping before anything else to make sure everything is more or less in order.
		if(false == ping(instanceClient.pClient, instanceClient.pServer))
		{
			error_print("Ping timeout.");
			result = -1;
			break;
		}

		// get server time
		time_t current_time;
		if(0 > ktools::serverTime(instanceClient, current_time) )
		{
			error_print("Failed to get server time.");
			result = -1;
			break;
		};
		{
			god::CGLock thelock(instanceGame.ServerTimeMutex);
			instanceGame.ServerTime = current_time;
		}

		// Disconnect if the game was closed.
		if( false == instanceGame.bRunning )
		{
			static const char send_buffer[] = "DISCONNECT\r\n";
			bytesTransmitted=-1;
			sendToConnection( instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer );
			if (bytesTransmitted == -1)
			{
				error_print("Error transmitting data.");
				result = -1;
				break;
			}
			break;
		}
		Sleep(100);
	}

	instanceGame.bRunning = false;
	disconnectClient(instanceClient);
	return result;
}