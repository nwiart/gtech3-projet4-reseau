#include "network.h"
#include "thread.h"

#include "Packet.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;



// Custom messages.
// Connection events are received through a silent window.
#define NET_ACCEPT (WM_USER)
#define NET_CLIENT (WM_USER+1)
#define NET_SERVER (WM_USER+2)

static const char g_wndServerClassName[] = "net_server_silent";
static const char g_wndClientClassName[] = "net_client_silent";



//
// Server structure.
//

// Server globals.
static struct serverinfo_t
{
	nsocket_t m_listenSocket;
	HWND m_hwnd;

	AcceptHandler m_acceptHandler;
	CloseHandler m_closeHandler;
	PacketHandler m_packetHandler;

	void* m_handlerParam;
}
g_server =
{
	INVALID_SOCKET,
	(HWND) INVALID_HANDLE_VALUE,
	0, 0,
	0, 0,
};

// Server event callbacks.
static LRESULT CALLBACK ServerWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case NET_ACCEPT:
		{
			sockaddr_in addr;
			int len = sizeof(addr);
			nsocket_t clientSocket = accept(g_server.m_listenSocket, (sockaddr*) &addr, &len);

			WSAAsyncSelect(clientSocket, hwnd, NET_CLIENT, FD_READ | FD_CLOSE);

			UCHAR* ip = (UCHAR*) &addr.sin_addr.S_un.S_un_b;
			std::cout << "New connection from " << (int)ip[0] << '.' << (int)ip[1] << '.' << (int)ip[2] << '.' << (int)ip[3] << ':' << addr.sin_port << '\n';

			g_server.m_acceptHandler(clientSocket, g_server.m_handlerParam);
		}
		break;

	case NET_CLIENT:
		{
			nsocket_t socket = (nsocket_t) wparam;

			switch (LOWORD(lparam))
			{
			case FD_CLOSE:
				{
					g_server.m_closeHandler(socket, g_server.m_handlerParam);

					closesocket(socket);
				}
				break;

			case FD_READ:
				{
					char buf[PACKET_MAX_SIZE];
					packet_recv(socket, buf);

					g_server.m_packetHandler(socket, *((const PacketBase*) buf), g_server.m_handlerParam);
				}
				break;
			}
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}



//
// Client structure.
//

// Client globals.
static PacketHandler g_clientPacketHandler = 0;
static CloseHandler g_clientCloseHandler = 0;
static void* g_clientHandlerParam = 0;
static HWND g_clientHwnd = (HWND) INVALID_HANDLE_VALUE;

// Client event callbacks.
static LRESULT CALLBACK ClientWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case NET_SERVER:
		{
			nsocket_t socket = (nsocket_t) wparam;

			switch (LOWORD(lparam))
			{
			case FD_CLOSE:
				{
					g_clientCloseHandler(socket, g_clientHandlerParam);

					closesocket(socket);
				}
				break;

			case FD_READ:
				{
					char buf[PACKET_MAX_SIZE];
					packet_recv(socket, buf);

					g_clientPacketHandler(socket, *((const PacketBase*) buf), g_clientHandlerParam);
				}
				break;
			}
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}



//
// Public API.
//

bool network_init()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		return false;
	}

	// Register classes for hidden windows.
	WNDCLASSEX wcex; ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = GetModuleHandle(0);
	wcex.lpszClassName = g_wndServerClassName;
	wcex.lpfnWndProc = ServerWinProc;
	RegisterClassEx(&wcex);

	wcex.lpszClassName = g_wndClientClassName;
	wcex.lpfnWndProc = ClientWinProc;
	RegisterClassEx(&wcex);

	return true;
}

void network_quit()
{
	UnregisterClass(g_wndServerClassName, GetModuleHandle(0));

	WSACleanup();
}

nsocket_t network_setup_server(uint16_t port, AcceptHandler acceptHandler, CloseHandler closeHandler, PacketHandler packetHandler, void* handlerParam)
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

	getaddrinfo(0, port_str, &hints, &result);
	nsocket_t listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(listenSocket, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(listenSocket, SOMAXCONN);

	g_server.m_listenSocket = listenSocket;
	g_server.m_acceptHandler = acceptHandler;
	g_server.m_closeHandler = closeHandler;
	g_server.m_packetHandler = packetHandler;
	g_server.m_handlerParam = handlerParam;

	g_server.m_hwnd = CreateWindow(g_wndServerClassName, "", 0, 0, 0, 100, 100, 0, 0, GetModuleHandle(0), 0);
	WSAAsyncSelect(listenSocket, g_server.m_hwnd, NET_ACCEPT, FD_ACCEPT);

	return listenSocket;
}

nsocket_t network_setup_web_server(uint16_t port)
{
	char port_str[6] = { 0 };
	_itoa_s(port, port_str, 10);

	// Create socket.
	addrinfo hints, * result;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(0, port_str, &hints, &result);
	nsocket_t listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(listenSocket, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(listenSocket, SOMAXCONN);

	return listenSocket;
}

void network_server_poll_events()
{
	if (g_server.m_hwnd == (HWND) INVALID_HANDLE_VALUE) return;

	MSG m;
	while (PeekMessage(&m, g_server.m_hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&m);
		DispatchMessage(&m);
	}
}



nsocket_t network_setup_client4(uint32_t addr, uint16_t port, CloseHandler closeHandler, PacketHandler packetHandler, void* handlerParam)
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

	g_clientPacketHandler = packetHandler;
	g_clientCloseHandler = closeHandler;
	g_clientHandlerParam = handlerParam;

	g_clientHwnd = CreateWindow(g_wndClientClassName, "", 0, 0, 0, 100, 100, 0, 0, GetModuleHandle(0), 0);
	WSAAsyncSelect(connectSocket, g_clientHwnd, NET_SERVER, FD_READ | FD_CLOSE);

	return connectSocket;
}

void network_shutdown_client(nsocket_t socket)
{
	closesocket(socket);
}

void network_client_poll_events()
{
	if (g_clientHwnd == (HWND) INVALID_HANDLE_VALUE) return;

	MSG m;
	while (GetMessage(&m, g_clientHwnd, 0, 0)) {
		TranslateMessage(&m);
		DispatchMessage(&m);
	}
}
