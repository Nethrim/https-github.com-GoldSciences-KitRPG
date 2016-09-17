#define NETLIB_SERVER_MODE_CLIENT
#include "draw.h"
#include "netlib.h"
#include "glabel.h"

#include <time.h>


#include <crtdbg.h>

// Use this function to draw our game data
void draw( klib::SGame& instanceGame ) // 
{
 	drawAndPresentGame(instanceGame);
};

#define MAX_SEND_SIZE 64

int runCommunications(klib::SGame& instanceGame);
int runCommunications(void* instanceGame)
{
	if(instanceGame)
		return runCommunications(*(klib::SGame*)instanceGame);

	return -1;
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

	klib::initASCIIScreen();
	
	if( initNetwork() )
	{
		fprintf(stderr, "Failed to initialize network\n");
		return -1;
	}

	klib::SGame* pInstancedGame = new klib::SGame;
	klib::SGame& instanceGame = *pInstancedGame;

	klib::initGame(instanceGame);

	runCommunications(instanceGame);

	while(instanceGame.bRunning)
	{
		pollInput(instanceGame.FrameInput);
		draw(instanceGame);
	}

	shutdownNetwork();

	if(pInstancedGame)
		delete(pInstancedGame);

	return 0;
}


int runCommunications(klib::SGame& instanceGame)
{
	SNetworkClient instanceClient;
	initClientConnection(instanceClient);

	// Send connection request
	char send_buffer[MAX_SEND_SIZE] = "CONNECT\r\n";

	int32_t bytesTransmitted=-1;
	sendToConnection( instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer );
	if (bytesTransmitted == -1)
	{
		fprintf(stderr, "Error transmitting data.\n");
		disconnectClient(instanceClient);
		return -1;
	}

	// Get response expecting a remote port to connect to.
	bytesTransmitted=-1;
	char connect_buffer[MAX_SEND_SIZE] = {};
	receiveFromConnection( instanceClient.pClient, connect_buffer, sizeof(connect_buffer), &bytesTransmitted, 0 );
	if( bytesTransmitted < 0 )
	{
		fprintf(stderr, "Error receiving data.\n");
		disconnectClient(instanceClient);
		return -1;
	}
	shutdownConnection(&instanceClient.pServer);

	fprintf(stderr, "response: %s\n", connect_buffer);

	god::error_t result = 0;
	// Got an available port to connect to? 
	if( 0 != strncmp(connect_buffer, "PORT:", 5) )
	{
		instanceGame.bRunning = false;
		shutdownConnection(&instanceClient.pClient);
		return -1;
	}

	int port = atoi(&connect_buffer[5]);
	instanceClient.pServer = 0;
	if( createConnection( instanceClient.a1, instanceClient.a2, instanceClient.a3, instanceClient.a4, port, &instanceClient.pServer ) )
	{
		fprintf(stderr, "Error creating new server connection.\n");
		return -1;
	}

	sprintf_s(send_buffer, "%s", "GET TIME\r\n");
	while(instanceGame.bRunning)
	{
		pollInput(instanceGame.FrameInput);
		draw(instanceGame);

		if(false == ping(instanceClient.pClient, instanceClient.pServer))
		{
			fprintf(stderr, "Ping timeout.\n");
			result = -1;
			break;
		}

		// send our command
		bytesTransmitted=-1;
		sendToConnection( instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer );
		if (bytesTransmitted == -1)
		{
			fprintf(stderr, "Error transmitting data.\n");
			result = -1;
			break;
		}

		/* Receive answer */
		bytesTransmitted=-1;
		char receive_buffer[256];
		receiveFromConnection( instanceClient.pClient, receive_buffer, sizeof(receive_buffer), &bytesTransmitted, 0 );
		//Sleep(1000);
		if( bytesTransmitted < 0 )
		{
			fprintf(stderr, "Error receiving data.\n");
			result = -1;
			break;
		}
		//printf("response: %s", buffer);		
		/* Display time */
		time_t current_time;						/* Time received */
		memcpy(&current_time, receive_buffer, sizeof(time_t));
		ctime_s(receive_buffer, sizeof(receive_buffer), &current_time);
		instanceGame.UserMessage = std::string("Server time: ") + receive_buffer;
		if( false == instanceGame.bRunning )
		{
			sprintf_s(send_buffer,"%s", "DISCONNECT\r\n");
			bytesTransmitted=-1;
			sendToConnection( instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer );
			if (bytesTransmitted == -1)
			{
				fprintf(stderr, "Error transmitting data.\n");
				result = -1;
				break;
			}
			//bTerminate = true;
			break;
		}
	}

	instanceGame.bRunning = false;
	disconnectClient(instanceClient);
	return result;
}