#include "mc/MC.h"
#include "WorldServer.h"

#include "mc/player/Player.h"

#include "mc/network/server/MCServer.h"
#include "mc/network/server/MCServerClient.h"

#include "network.h"
#include "Packet.h"



WorldServer::WorldServer(int sizeX, int sizeY, MCServer* server)
	: World(sizeX, sizeY, true)
	, m_server(server)
{

}

void WorldServer::movePlayer(const MCServerClient& player, int dx, int dy)
{
	World::movePlayer(player, dx, dy);

	Packet<ServerPlayerMovePacket> playerMovePacket;
	playerMovePacket->m_playerID = player.getPlayerID();
	playerMovePacket->m_posX = player.getPlayer()->getPosX();
	playerMovePacket->m_posY = player.getPlayer()->getPosY();

	m_server->broadcastPacket(playerMovePacket, player.getSocket());
}

void WorldServer::breakTile(int x, int y)
{
	World::breakTile(x, y);

	Packet<ServerBlockBreakPacket> blockBreakPacket;
	blockBreakPacket->m_posX = x;
	blockBreakPacket->m_posY = y;
	blockBreakPacket->m_item = this->getItemAt(x, y);
	
	m_server->broadcastPacket(blockBreakPacket);
}

