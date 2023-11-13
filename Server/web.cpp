#include "web.h"

#include "network.h"
#include "thread.h"


#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;



static nsocket_t g_webSocket;

static string g_webPage;



static int pageSendThread(void*)
{
	while (true)
	{
		// Get incoming HTTP request.
		nsocket_t incoming = accept(g_webSocket, 0, 0);

		char a[4096] = { 0 };
		recv(incoming, a, 4096, 0);

		// Compose page.
		stringstream buf;
		{
			ifstream f("index.html");
		
			buf <<
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n";

			buf << f.rdbuf();
		}

		send(incoming, buf.str().c_str(), buf.str().size(), 0);

		// End of transfer.
		closesocket(incoming);
	}

	return 0;
}

thread t(pageSendThread);


void web_start_server(uint16_t port)
{
	g_webSocket = network_setup_server(port);

	ifstream f("index.html");
	stringstream buf;
	buf << f.rdbuf();

	g_webPage = buf.str();

	t.start();
}
