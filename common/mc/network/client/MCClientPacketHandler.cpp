#include "mc/MC.h"
#include "MCClientPacketHandler.h"

#include "mc/world/World.h"

#include <iostream>


World* world = 0;


void MCClientPacketHandler::response(nsocket_t socket, const PacketBase& b)
{
	switch (b.m_id)
	{
	case ServerPackets::ConnectionResponse:
		{
			const Packet<ConnectionResponsePacket>& p = (const Packet<ConnectionResponsePacket>&) b;
			std::cout << p->m_reason.toString() << '\n';
		}
		break;

	case ServerPackets::GetWorldDimensions:
		{
			const Packet<ServerGetWorldDimensionsPacket>& p = (const Packet<ServerGetWorldDimensionsPacket>&) b;
			world = new World(p->m_sizeX, p->m_sizeY);
		}
		break;

	case ServerPackets::GetWorldTiles:
		{
			const Packet<ServerGetWorldTilesPacket>& p = (const Packet<ServerGetWorldTilesPacket>&) b;
			world->setTileRange(p->m_tiles, p->m_numTiles, p->m_startIndex);
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
			const Packet<ServerDisconnectPacket>& p = (const Packet<ServerDisconnectPacket>&) b;
			std::cout << p->m_reason.toString() << '\n';
		}
		break;
	}
}
