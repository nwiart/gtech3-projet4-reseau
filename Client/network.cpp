#include "network.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <iostream>
using namespace std;



// Connection socket for the client, listening socket for the server.
static SOCKET g_socket = INVALID_SOCKET;



void network_initialize()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void network_shutdown()
{
	if (g_socket != INVALID_SOCKET) {
		closesocket(g_socket);
	}

	WSACleanup();
}



void network_connect(const char* port)
{
	addrinfo hints, * result;
	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("192.168.43.250", "27015", &hints, &result);
	g_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	cout << "Trying to reach server...\n";
	while (connect(g_socket, result->ai_addr, result->ai_addrlen) != 0);

	freeaddrinfo(result);
}

void network_setup_server(const char* port)
{

}
