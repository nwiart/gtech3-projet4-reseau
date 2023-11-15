#include "mc/MC.h"
#include "MCServer.h"

#include "mc/network/server/MCServerPacketHandler.h"
#include "mc/network/client/MCClient.h"

#include "mc/world/World.h"

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
	server->run();

	delete server;

	network_quit();

	return 0;
}



MCServer::MCServer(bool headless)
	: m_adminClient(0)
	, m_adminClientThread(&MCServer::adminClientThreadMain, this)
	, m_headless(headless)
{
	
}

MCServer::~MCServer()
{
	
}

void MCServer::host()
{
	cout << "TIC TAC TOE Server\n";
	cout << "---\n\n";

	// Start game server.
	nsocket_t listenSocket = network_setup_server(MC::SERVER_PORT, &MCServerPacketHandler::response);
	cout << "Now listening on " << MC::SERVER_PORT << "...\n";

	// Start web server.
	web_start_server(MC::WEB_PORT);
	cout << "Web server broadcasting on " << MC::WEB_PORT << "...\n";
}

void MCServer::run()
{
	if (!m_headless) {
		m_adminClientThread.start();
	}

	World* world = new World(16, 16);
	

	while (m_adminClientThread.isRunning())
	{
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
}

void MCServer::sendPacket(? ? , const PacketBase& b)
{
	packet_send(?, b);
}

void MCServer::broadcastPacket(const PacketBase& b)
{
	for () {
		packet_send(? , b);
	}
}

void MCServer::onPlayerConnect(nsocket_t socket)
{
	int numTilesToSend = 16 * 16;
	int numSent = 0;

	while (numTilesToSend != 0) {
		Packet<ServerGetWorldTilesPacket> p;
		p->m_startIndex = numSent;

		if (numTilesToSend >= 256) {
			p->m_numTiles = 256;
			world->getTileRange(p->m_tiles, 256, numSent);

			numTilesToSend -= 256;
			numSent += 256;
		}
		else {
			p->m_numTiles = numTilesToSend;
			world->getTileRange(p->m_tiles, numTilesToSend, numSent);

			numTilesToSend = 0;
		}

		this->sendPacket(socket, p);
	}
}


int MCServer::adminClientThreadMain(void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	server->m_adminClient = new MCClient("MC Server");

	server->m_adminClient->connect("127.0.0.1", MC::SERVER_PORT);
	server->m_adminClient->run();

	return 0;
}
