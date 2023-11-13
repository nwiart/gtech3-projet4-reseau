#include "network.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <stdlib.h>

#include <string>
#include <sstream>
using namespace std;



bool network_init()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	return result == 0;
}

void network_quit()
{
	WSACleanup();
}


nsocket_t network_setup_server(uint16_t port)
{
	char port_str[6] = { 0 };
	_itoa_s(port, port_str, 10);

	// Create socket.
	addrinfo hints, *result;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo("127.0.0.1", port_str, &hints, &result);
	nsocket_t listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(listenSocket, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(listenSocket, SOMAXCONN);

	return listenSocket;
}



nsocket_t network_setup_client4(uint32_t addr, uint16_t port)
{
	// Convert to string.
	char port_str[6] = { 0 };
	_itoa_s(port, port_str, 10);

	stringstream address_str;
	address_str << ((addr >> 24) & 0xFF) << '.' << ((addr >> 16) & 0xFF) << '.' << ((addr >> 8) & 0xFF) << '.' << ((addr) & 0xFF);

	// Create socket.
	addrinfo hints, *result;
	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(address_str.str().c_str(), port_str, &hints, &result);
	nsocket_t connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	while (connect(connectSocket, result->ai_addr, result->ai_addrlen) != 0);

	freeaddrinfo(result);

	return connectSocket;
}
