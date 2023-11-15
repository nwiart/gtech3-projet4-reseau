#include "mc/MC.h"
#include "MCServer.h"

#include "mc/network/server/MCServerPacketHandler.h"
#include "mc/network/client/MCClient.h"

#include "web.h"

#include <Windows.h>

#include <iostream>
using namespace std;



int MCServer::main(bool headless)
{
	if (!network_init()) {
		std::cout << "Network initialization failed!\n";
		return 1;
	}

	MCServer* server = new MCServer(headless);

	server->host();


	while (1) {
		network_server_poll_events();

		Sleep(1);
	}

	/*string s;
	while (true) {
		cout << "> ";
		getline(cin, s);

		if (s == "exit") {
			break;
		}
	}*/


	delete server;

	network_quit();

	return 0;
}



MCServer::MCServer(bool headless)
	: m_adminClient(0)
{
	if (!headless)
	{
		m_adminClient = new MCClient();
	}
}

MCServer::~MCServer()
{
	
}

void MCServer::host()
{
	cout << "TIC TAC TOE Server\n";
	cout << "---\n\n";


	nsocket_t listenSocket = network_setup_server(MC::SERVER_PORT, &MCServerPacketHandler::response);

	cout << "Now listening on " << MC::SERVER_PORT << "...\n";


	web_start_server(MC::WEB_PORT);
	cout << "Web server broadcasting on " << MC::WEB_PORT << "...\n";
}
