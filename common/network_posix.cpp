#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "network.h"



bool network_init()
{
	// No initialization needed.
	return true;
}

void network_quit()
{

}


nsocket_t network_setup_server(uint16_t port)
{
	sockaddr_in addr;

	nsocket_t listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	bind(listenSocket, (sockaddr*) &addr, sizeof(addr));

	listen(listenSocket, 4);

	socklen_t addrLen;
	nsocket_t clientSocket = accept(listenSocket, &addr, &addrLen);

	return listenSocket;
}
