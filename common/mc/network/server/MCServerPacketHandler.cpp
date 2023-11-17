#include "mc/MC.h"
#include "MCServerPacketHandler.h"

#include "mc/network/server/MCServer.h"
#include "mc/network/server/world/WorldServer.h"

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
	case ClientPackets::Join:
		{
			std::cout << "Client sent Join\n";

			const Packet<ClientJoinPacket>& p = *((const Packet<ClientJoinPacket>*) & b);

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

			std::string name = p->m_name;
			server->onPlayerConnect(client, name.c_str());
		}
		break;

	case ClientPackets::PlayerMove:
		{
			std::cout << "Client sent PlayerMove\n";

			const Packet<ClientPlayerMovePacket>& p = (const Packet<ClientPlayerMovePacket>&) b;

			const MCServerClient& clientInfo = server->getClient(client);
			MC::getInstance().getWorld()->movePlayer(clientInfo, p->m_dx, p->m_dy);
		}
		break;
	}
}
