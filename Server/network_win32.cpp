#include "network.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <stdlib.h>



bool network_init()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void network_quit()
{
	WSACleanup();
}


nsocket_t network_setup_server(uint16_t port)
{
	char port_str[6] = { 0 };
	itoa(port, port_str, 10);

	addrinfo hints, *result;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(0, port_str, &hints, &result);
	nsocket_t listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(listenSocket, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(listenSocket, 4);

	return listenSocket;
}
