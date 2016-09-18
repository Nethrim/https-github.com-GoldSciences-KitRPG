#include "netlib_client.h"
#include "netlib_private.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "GCore_printf.h"

using namespace ktools;

int32_t ktools::initClientConnection(SNetworkClient& instanceClient)
{

	if( createConnection( instanceClient.b1, instanceClient.b2, instanceClient.b3, instanceClient.b4, 0, &instanceClient.pClient ) )
	{
		error_printf("Error creating client connection.");
		return -1;
	}

	if( initConnection(instanceClient.pClient) )
	{
		error_printf("Error initializing client connection.");
		return -1;
	}

	if( bindConnection(instanceClient.pClient) )
	{
		error_printf("Error binding client connection.");
		shutdownConnection(&instanceClient.pClient);
		return -1;
	}

	if( createConnection( instanceClient.a1, instanceClient.a2, instanceClient.a3, instanceClient.a4, instanceClient.port_number, &instanceClient.pServer ) )
	{
		error_printf("Error creating new connection to server.");
		return -1;
	}

	return 0;
}


void ktools::disconnectClient(SNetworkClient& client)
{
	/* Get current time */
	debug_print("Disconnecting client.");
	shutdownConnection(&client.pClient);
	shutdownConnection(&client.pServer);
}

#define MAX_SEND_SIZE 128

int ktools::connect(SNetworkClient& instanceClient)
{
	// Send connection request
	char send_buffer[] = "CONNECT\r\n";

	int32_t bytesTransmitted=-1;
	sendToConnection(instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer);
	if (bytesTransmitted == -1)
	{
		error_print("Error transmitting data.");
		disconnectClient(instanceClient);
		return -1;
	}

	// Get response expecting a remote port to connect to.
	bytesTransmitted=-1;
	char connect_buffer[MAX_SEND_SIZE] = {};
	receiveFromConnection(instanceClient.pClient, connect_buffer, sizeof(connect_buffer), &bytesTransmitted, 0);
	if( bytesTransmitted < 0 )
	{
		error_print("Error receiving data.");
		disconnectClient(instanceClient);
		return -1;
	}
	shutdownConnection(&instanceClient.pServer);

	debug_printf("response: %s", connect_buffer);

	// Got an available port to connect to? 
	if( 0 != strncmp(connect_buffer, "PORT:", 5) )
	{
		shutdownConnection(&instanceClient.pClient);
		return -1;
	}

	int port = atoi(&connect_buffer[5]);
	instanceClient.pServer = 0;
	if(createConnection(instanceClient.a1, instanceClient.a2, instanceClient.a3, instanceClient.a4, port, &instanceClient.pServer))
	{
		error_print("Error creating new server connection.");
		return -1;
	}
	return 0;
}

int ktools::serverTime(SNetworkClient& instanceClient, time_t& current_time)
{
	static const char send_buffer[] = "GET TIME\r\n";
	// send our command
	int32_t bytesTransmitted=-1;
	sendToConnection(instanceClient.pClient, send_buffer, (int)strlen(send_buffer) + 1, &bytesTransmitted, instanceClient.pServer);
	if (bytesTransmitted == -1)
	{
		error_print("Error transmitting data.");
		return -1;
	}

	// Receive answer
	bytesTransmitted=-1;
	char receive_buffer[sizeof(time_t)*2] = {};
	receiveFromConnection(instanceClient.pClient, receive_buffer, sizeof(receive_buffer), &bytesTransmitted, 0);

	if( bytesTransmitted < 0 )
	{
		error_print("Error receiving data.");
		return -1;
	}

	memcpy(&current_time, receive_buffer, sizeof(time_t));
	return 0;
}