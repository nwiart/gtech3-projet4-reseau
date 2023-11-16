#include "mc/MC.h"
#include "MCServer.h"

#include "mc/network/server/MCServerPacketHandler.h"
#include "mc/network/client/MCClient.h"

#include "mc/network/server/MCServerClient.h"

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
	, m_currentPlayerID(0)
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
	nsocket_t listenSocket = network_setup_server(MC::SERVER_PORT, &MCServerPacketHandler::handleAccept, &MCServerPacketHandler::handleClose, &MCServerPacketHandler::handlePacket, this);
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

void MCServer::disconnectPlayer(nsocket_t socket, DisconnectReason reason)
{
	ClientList::iterator it = m_clients.find(socket);
	if (it == m_clients.end()) {
		return;
	}

	// Player not spawned in, probably got refused by the server.
	if (it->second.getPlayer()) {

		// Send disconnect reason.
		Packet<ServerDisconnectPacket> disconnectPacket;
		disconnectPacket->m_playerID = it->second.getPlayerID();
		disconnectPacket->m_reason = reason;

		this->broadcastPacket(disconnectPacket, socket);

		// TODO : Despawn player.
		//it->second.getPlayer()
	}

	m_clients.erase(it);
}

void MCServer::sendPacket(nsocket_t socket, const PacketBase& b)
{
	packet_send(socket, b);
}

void MCServer::broadcastPacket(const PacketBase& b, nsocket_t socketExcept)
{
	for (const ClientListElement& c : m_clients) {
		if (c.first == socketExcept) continue;

		packet_send(c.first, b);
	}
}

void MCServer::onAccept(nsocket_t socket)
{
	// Register client connection.
	m_clients.insert(ClientListElement(socket, MCServerClient(socket)));
}

void MCServer::onClose(nsocket_t socket)
{
	this->disconnectPlayer(socket, DisconnectReason::PLAYER_LEFT);
}

void MCServer::onPlayerConnect(nsocket_t socket, const char* name)
{
	int numTilesToSend = 0;//16 * 16;
	int numSent = 0;

	// TODO : Spawn player.
	MCServerClient client(socket);


	// Send world dimensions.
	Packet<ServerGetWorldDimensionsPacket> getWorldDimensionsPacket;
	getWorldDimensionsPacket->m_sizeX = 16;
	getWorldDimensionsPacket->m_sizeY = 16;
	this->sendPacket(socket, getWorldDimensionsPacket);

	// Send world tiles to player.
	while (numTilesToSend != 0) {
		Packet<ServerGetWorldTilesPacket> p;
		p->m_startIndex = numSent;

		if (numTilesToSend >= 256) {
			p->m_numTiles = 256;
			MC::getInstance().getWorld()->getTileRange(p->m_tiles, 256, numSent);

			numTilesToSend -= 256;
			numSent += 256;
		}
		else {
			p->m_numTiles = numTilesToSend;
			MC::getInstance().getWorld()->getTileRange(p->m_tiles, numTilesToSend, numSent);

			numTilesToSend = 0;
		}

		this->sendPacket(socket, p);
	}

	// Send assigned player ID.
	Packet<ServerGetPlayerIDPacket> getPlayerIDPacket;
	getPlayerIDPacket->m_playerID = m_currentPlayerID;
	this->sendPacket(socket, getPlayerIDPacket);

	// Notify other players of spawn.
	Packet<ServerPlayerSpawnPacket> playerSpawnPacket;
	playerSpawnPacket->m_playerID = m_currentPlayerID;
	strcpy_s(playerSpawnPacket->m_playerName, name);

	this->broadcastPacket(playerSpawnPacket, socket);


	m_currentPlayerID++;
}


int MCServer::adminClientThreadMain(void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	server->m_adminClient = new MCClient("MC Server");

	server->m_adminClient->connect("127.0.0.1", MC::SERVER_PORT);
	server->m_adminClient->run();

	return 0;
}
