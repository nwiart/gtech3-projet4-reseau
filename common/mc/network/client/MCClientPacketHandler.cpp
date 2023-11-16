#include "mc/MC.h"
#include "MCClientPacketHandler.h"

#include "mc/network/client/MCClient.h"
#include "mc/world/World.h"

#include <iostream>


static World* world = 0;


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
			world = new World(p->m_sizeX, p->m_sizeY);
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

			world->spawnLocalPlayer(p->m_playerID);
			MC::getInstance().openWorld(world);
		}
		break;

	case ServerPackets::PlayerSpawn:
		{
			const Packet<ServerPlayerSpawnPacket>& p = (const Packet<ServerPlayerSpawnPacket>&) b;
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

		}
		break;

	case ServerPackets::Disconnect:
		{
			std::cout << "Server sent Disconnect\n";

			const Packet<ServerDisconnectPacket>& p = (const Packet<ServerDisconnectPacket>&) b;
			std::cout << p->m_reason.toString() << '\n';
		}
		break;
	}
}
