#include "mc/MC.h"
#include "MCServer.h"

#include "mc/network/server/MCServerPacketHandler.h"
#include "mc/network/server/MCServerClient.h"
#include "mc/network/client/MCClient.h"

#include "mc/network/server/world/WorldServer.h"
#include "mc/player/Player.h"

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

	srand(time(0));

	MC mc;
	mc.init();

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
	// Create dummy world and set current.
	WorldServer* world = new WorldServer(31, 47, this);
	MC::getInstance().openWorld(0, world);

	if (!m_headless) {
		m_adminClientThread.start();
	}

	//while (m_adminClientThread.isRunning())
	while (1)
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

		// Despawn player.
		MC::getInstance().getWorld()->despawnPlayer(it->second.getPlayerID());
	}

	m_clients.erase(it);

	std::cout << "Player disconnected (" << reason.toString() << ")\n";
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

const MCServerClient& MCServer::getClient(nsocket_t socket) const
{
	return m_clients.find(socket)->second;
}

MCServerClient& MCServer::getClient(nsocket_t socket)
{
	return m_clients.find(socket)->second;
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
	World* world = MC::getInstance().getWorld();

	// Spawn player and update client info.
	MCServerClient& clientInfo = this->getClient(socket);
	clientInfo.m_playerID = m_currentPlayerID;
	clientInfo.m_player = world->spawnRemotePlayer(m_currentPlayerID, world->getSizeX() / 2, 0);

	// Send world dimensions.
	Packet<ServerGetWorldDimensionsPacket> getWorldDimensionsPacket;
	getWorldDimensionsPacket->m_sizeX = world->getSizeX();
	getWorldDimensionsPacket->m_sizeY = world->getSizeY();
	this->sendPacket(socket, getWorldDimensionsPacket);

	// Send world tiles to player.
	int numTilesToSend = world->getSizeX() * world->getSizeY();
	int numSent = 0;

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
	playerSpawnPacket->m_xPos = clientInfo.getPlayer()->getPosX();
	playerSpawnPacket->m_yPos = clientInfo.getPlayer()->getPosY();
	strcpy_s(playerSpawnPacket->m_playerName, name);

	this->broadcastPacket(playerSpawnPacket, socket);

	m_currentPlayerID++;
}

void MCServer::onPlayerMove(nsocket_t socket, int dx, int dy)
{
	auto it = m_clients.find(socket);

	it->second.getPlayer()->move(dx, dy);
}


int MCServer::adminClientThreadMain(void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	server->m_adminClient = new MCClient("MC Server");

	server->m_adminClient->connect("127.0.0.1", MC::SERVER_PORT);
	server->m_adminClient->run();

	delete server->m_adminClient;

	return 0;
}
