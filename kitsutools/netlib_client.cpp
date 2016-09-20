#include "netlib_client.h"
#include "netlib_private.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "GCore_printf.h"

using namespace ktools;

int32_t ktools::initClientConnection(SNetworkClient& instanceClient)
{

	if( createConnection( instanceClient.l1, instanceClient.l2, instanceClient.l3, instanceClient.l4, 0, &instanceClient.pClient ) )
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

	if( createConnection( instanceClient.r1, instanceClient.r2, instanceClient.r3, instanceClient.r4, instanceClient.rPort, &instanceClient.pServer ) )
	{
		error_printf("Error creating new connection to server.");
		return -1;
	}

	return 0;
}

int32_t ktools::requestDisconnect(SNetworkClient& instanceClient)
{
	static const NETLIB_COMMAND commandDisconnect = NETLIB_COMMAND_DISCONNECT;
	int32_t bytesTransmitted=-1;
	sendToConnection( instanceClient.pClient, (const char*)&commandDisconnect, (int)sizeof(NETLIB_COMMAND), &bytesTransmitted, instanceClient.pServer );
	if (bytesTransmitted == -1)
	{
		error_print("Error sending disconnect message.");
		return -1;
	}
	debug_print("Disconnect message sent successfully.");
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
	static const NETLIB_COMMAND send_buffer = NETLIB_COMMAND_CONNECT;

	int32_t bytesTransmitted=-1;
	sendToConnection(instanceClient.pClient, (const char*)&send_buffer, (int)sizeof(NETLIB_COMMAND), &bytesTransmitted, instanceClient.pServer);
	if (bytesTransmitted != sizeof(NETLIB_COMMAND))
	{
		error_print("Error transmitting data.");
		disconnectClient(instanceClient);
		return -1;
	}

	// Get response expecting a remote port to connect to.
	bytesTransmitted=-1;
	NETLIB_COMMAND connect_buffer = NETLIB_COMMAND_INVALID;
	receiveFromConnection(instanceClient.pClient, (char*)&connect_buffer, sizeof(NETLIB_COMMAND), &bytesTransmitted, 0);
	if( connect_buffer != NETLIB_COMMAND_PORT )
	{
		error_print("Error receiving data.");
		disconnectClient(instanceClient);
		return -1;
	}
	shutdownConnection(&instanceClient.pServer);

	debug_printf("response: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(connect_buffer).c_str());

	int32_t port_buffer = -1;
	receiveFromConnection(instanceClient.pClient, (char*)&port_buffer, sizeof(int32_t), &bytesTransmitted, 0);
	debug_printf("port reported available: %i.", port_buffer);

	// Got an available port to connect to? 
	if(createConnection(instanceClient.r1, instanceClient.r2, instanceClient.r3, instanceClient.r4, port_buffer, &instanceClient.pServer))
	{
		error_print("Error creating new server connection.");
		return -1;
	}
	debug_print("Connection created.");
	return 0;
}

int ktools::serverTime(SNetworkClient& instanceClient, uint64_t& current_time)
{
	// send our command
	if(0 > sendSystemCommand(instanceClient.pClient, instanceClient.pServer, NETLIB_COMMAND_TIME_GET))
	{
		error_print("Error transmitting data.");
		return -1;
	}

	// Receive answer
	NETLIB_COMMAND commandTimeSet = NETLIB_COMMAND_INVALID;
	if(0 > receiveSystemCommand(instanceClient.pClient, instanceClient.pServer, commandTimeSet))
	{
		error_print("Error transmitting data.");
		return -1;
	}
	
	debug_printf("response: %s.", god::genum_definition<NETLIB_COMMAND>::get().get_value_label(commandTimeSet).c_str());

	uint64_t receive_time=-1;
	int32_t bytesTransmitted = -1;
	receiveFromConnection(instanceClient.pClient, (char*)&receive_time, sizeof(receive_time), &bytesTransmitted, 0);

	memcpy(&current_time, &receive_time, sizeof(uint64_t));
	return 0;
}
