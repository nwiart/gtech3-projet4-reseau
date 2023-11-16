#include "mc/MC.h"
#include "MCServerPacketHandler.h"

#include "mc/network/server/MCServer.h"

#include <string>
#include <iostream>



void MCServerPacketHandler::handleAccept(nsocket_t socket, void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	server->onAccept(socket);
}

void MCServerPacketHandler::handleClose(nsocket_t socket, void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	server->onClose(socket);
}

void MCServerPacketHandler::handlePacket(nsocket_t client, const PacketBase& b, void* param)
{
	MCServer* server = reinterpret_cast<MCServer*>(param);

	switch (b.m_id)
	{
	case ClientPackets::SetName:
		{
			const Packet<SetNamePacket>& p = *((const Packet<SetNamePacket>*) & b);
			std::cout << "Client sent SetName\n";

			ConnectionDenialReason reason = ConnectionDenialReason::ALLOWED;
			
			//reason = ConnectionDenialReason::BAD_NAME;

			// Send connection response packet.
			Packet<ConnectionResponsePacket> responsePacket;
			responsePacket->m_reason = reason;
			packet_send(client, responsePacket);

			if (reason != ConnectionDenialReason::ALLOWED) {
				server->disconnectPlayer(client, DisconnectReason::UNKNOWN);
				break;
			}

			std::string name = p->name;
			server->onPlayerConnect(client, name.c_str());
		}
		break;

	case ClientPackets::PlayerMove:
		break;
	}
}
