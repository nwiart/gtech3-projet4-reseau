#include "web.h"

#include "network.h"
#include "thread.h"


#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



static nsocket_t g_webSocket;

static string read(istream& stream, uint32_t count)
{
	vector<char> result(count + 1, 0);
	stream.read(&result[0], count);

	return string(result.data());
}

static void make_page(std::string& out, const char* buf)
{
	char c;

	while ((c = *buf) && c) {

		if (c == '$') {
			buf++;

			string identifier;

			while ((c = *buf) && (isalnum(c))) {
				identifier += c;
				buf++;
			}

			if (identifier.empty()) out += '$';
			else if (identifier == "onlinePlayers") out += "3";
			else if (identifier == "random") out += std::to_string(rand());
		}

		else {
			out += c;
			buf++;
		}
	}
}



static int pageSendThread(void*)
{
	while (true)
	{
		// Get incoming HTTP request.
		nsocket_t incoming = accept(g_webSocket, 0, 0);

		char a[4096] = { 0 };
		recv(incoming, a, 4096, 0);

		// Compose page.
		string page;
		{
			page =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"\r\n";


			ifstream f("index.html");
			stringstream buf;
			buf << f.rdbuf();
			string s = buf.str();

			make_page(page, s.c_str());
		}

		send(incoming, page.c_str(), page.size(), 0);

		// End of transfer.
		closesocket(incoming);
	}

	return 0;
}

static thread t(pageSendThread);

void web_start_server(uint16_t port)
{
	g_webSocket = network_setup_web_server(port);

	t.start();
}
