#include "mc/MC.h"
#include "WorldServer.h"

#include "mc/player/Player.h"
#include "mc/player/Inventory.h"

#include "mc/network/server/MCServer.h"
#include "mc/network/server/MCServerClient.h"

#include "network.h"
#include "Packet.h"



WorldServer::WorldServer(int sizeX, int sizeY, MCServer* server)
	: World(sizeX, sizeY, true)
	, m_server(server)
{

}

void WorldServer::movePlayer(const MCServerClient& playerInfo, int dx, int dy)
{
	World::movePlayer(playerInfo, dx, dy);

	Player* player = playerInfo.getPlayer();
	uint16_t itemID = this->getItemAt(player->getPosX(), player->getPosY());
	if (itemID != 0) {
		player->getInventory()->addItem(itemID, 1);
		this->setItemAt(0, player->getPosX(), player->getPosY());

		Packet<ServerPlayerPickupItemPacket> playerPickupItemPacket;
		playerPickupItemPacket->m_playerID = playerInfo.getPlayerID();
		playerPickupItemPacket->m_posX = player->getPosX();
		playerPickupItemPacket->m_posY = player->getPosY();

		m_server->broadcastPacket(playerPickupItemPacket);
	}

	Packet<ServerPlayerMovePacket> playerMovePacket;
	playerMovePacket->m_playerID = playerInfo.getPlayerID();
	playerMovePacket->m_posX = player->getPosX();
	playerMovePacket->m_posY = player->getPosY();

	m_server->broadcastPacket(playerMovePacket, playerInfo.getSocket());
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
