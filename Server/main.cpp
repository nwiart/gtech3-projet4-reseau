#include "mc/MC.h"
#include "mc/network/server/MCServer.h"



static void disconnect_client(nsocket_t client)
{
	
	//closesocket(client);
}



int main(int argc, char** argv)
{
	return MCServer::main(false);
}
