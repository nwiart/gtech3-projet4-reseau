#include "web.h"

#include "network.h"
#include "thread.h"

#include "mc/network/server/MCServer.h"
#include "mc/player/Player.h"


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

static void make_page(std::string& out, const char* buf, MCServer* server)
{
	char c;
	const char* p = buf;

	while ((c = *buf) && c) {

		if (c == '$') {
			out += string(p, buf);

			buf++;

			string identifier;
			while ((c = *buf) && (isalnum(c))) {
				identifier += c;
				buf++;
			}

			if (identifier.empty()) out += '$';
			else if (identifier == "onlinePlayers") out += std::to_string(server->getClients().size());
			else if (identifier == "maximumPlayers") out += server->getServerConfig().find("maximumPlayers")->second;
			else if (identifier == "playerList") {
				for (const MCServer::ClientListElement& e : server->getClients()) out += e.second.getPlayer()->getName() + '\n';
			}
			else if (identifier == "random") out += std::to_string(rand());

			p = buf;
		}

		else {
			buf++;
		}
	}

	if (p != buf) {
		out += string(p, buf);
	}
}



static int pageSendThread(void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

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

			make_page(page, s.c_str(), server);
		}

		send(incoming, page.c_str(), page.size(), 0);

		// End of transfer.
		closesocket(incoming);
	}

	return 0;
}

void web_start_server(uint16_t port, void* param)
{
	g_webSocket = network_setup_web_server(port);

	static thread t(pageSendThread, param);
	t.start();
}
