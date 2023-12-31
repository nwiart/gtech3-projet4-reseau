#include "mc/MC.h"
#include "MCClientPacketHandler.h"

#include "mc/network/client/MCClient.h"

#include "mc/world/World.h"
#include "mc/player/Player.h"
#include "mc/player/Inventory.h"

#include "mc/gui/GuiInventory.h"

#include <iostream>



static World* world = 0;



void MCClientPacketHandler::handleClose(nsocket_t socket, void* param)
{
	MCClient* client = reinterpret_cast<MCClient*>(param);

	client->onServerClose();

	MC::getInstance().openLocalWorld(0);
	delete world;
}

void MCClientPacketHandler::handlePacket(nsocket_t socket, const PacketBase& b, void* param)
{
	MCClient* client = reinterpret_cast<MCClient*>(param);

	switch (b.m_id)
	{
	case ServerPackets::ConnectionResponse:
		{
			std::cout << "Server sent ConnectionResponse\n";

			const Packet<ConnectionResponsePacket>& p = (const Packet<ConnectionResponsePacket>&) b;
			std::cout << p->m_reason.toString() << '\n';
		}
		break;

	case ServerPackets::GetWorldDimensions:
		{
			std::cout << "Server sent GetWorldDimensions\n";

			const Packet<ServerGetWorldDimensionsPacket>& p = (const Packet<ServerGetWorldDimensionsPacket>&) b;
			world = new World(p->m_sizeX, p->m_sizeY, false);
		}
		break;

	case ServerPackets::GetWorldTiles:
		{
			std::cout << "Server sent GetWorldTiles\n";

			const Packet<ServerGetWorldTilesPacket>& p = (const Packet<ServerGetWorldTilesPacket>&) b;
			world->setTileRange(p->m_tiles, p->m_numTiles, p->m_startIndex);
		}
		break;

	case ServerPackets::GetPlayerID:
		{
			std::cout << "Server sent GetPlayerID\n";

			const Packet<ServerGetPlayerIDPacket>& p = (const Packet<ServerGetPlayerIDPacket>&) b;

			Player* player = world->spawnLocalPlayer(p->m_playerID);
			MC::getInstance().openLocalWorld(world);

			client->m_inventoryUI = new GuiInventory(player->getInventory());
			MCClient::getInstance().m_showStatus = false;
		}
		break;

	case ServerPackets::PlayerSpawn:
		{
			std::cout << "Server sent PlayerSpawn\n";

			const Packet<ServerPlayerSpawnPacket>& p = (const Packet<ServerPlayerSpawnPacket>&) b;
			world->spawnRemotePlayer(p->m_playerID, p->m_playerName, p->m_xPos, p->m_yPos);
		}
		break;

	case ServerPackets::GameWin:
		{

		}
		break;

	case ServerPackets::GameRestart:
		{

		}
		break;

	case ServerPackets::PlayerMove:
		{
			std::cout << "Server sent PlayerMove\n";

			const Packet<ServerPlayerMovePacket>& p = (const Packet<ServerPlayerMovePacket>&) b;

			World* world = MC::getInstance().getLocalWorld();
			world->moveRemotePlayer(p->m_playerID, p->m_posX, p->m_posY);
		}
		break;

	case ServerPackets::BlockBreak:
		{
			std::cout << "Server sent BlockBreak\n";

			const Packet<ServerBlockBreakPacket>& p = (const Packet<ServerBlockBreakPacket>&) b;

			World* world = MC::getInstance().getLocalWorld();
			world->setItemAt(p->m_item, p->m_posX, p->m_posY);
			world->breakTile(p->m_posX, p->m_posY);
		}
		break;

	case ServerPackets::PlayerPickupItem:
		{
			std::cout << "Server sent PlayerPickupItem\n";

			const Packet<ServerPlayerPickupItemPacket>& p = (const Packet<ServerPlayerPickupItemPacket>&) b;

			World* world = MC::getInstance().getLocalWorld();
			Player* player = MC::getInstance().getLocalPlayer();

			if (p->m_playerID == player->getID()) {
				uint16_t itemID = world->getItemAt(p->m_posX, p->m_posY);
				player->getInventory()->addItem(itemID, 1);
			}
			world->setItemAt(0, p->m_posX, p->m_posY);
		}
		break;

	case ServerPackets::Disconnect:
		{
			std::cout << "Server sent Disconnect\n";

			const Packet<ServerDisconnectPacket>& p = (const Packet<ServerDisconnectPacket>&) b;
			
			World* world = MC::getInstance().getLocalWorld();
			world->despawnPlayer(p->m_playerID);
		}
		break;
	}
}
