#include "mc/MC.h"
#include "MCClientPacketHandler.h"

#include <iostream>



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

	case ServerPackets::GameWin:
		{

		}
		break;

	case ServerPackets::GameRestart:
		{

		}
		break;

	case ServerPackets::Play:
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
