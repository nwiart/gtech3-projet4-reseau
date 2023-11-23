#include "mc/MC.h"
#include "MCServerPacketHandler.h"

#include "mc/network/server/MCServer.h"
#include "mc/network/server/world/WorldServer.h"

#include "mc/player/Player.h"

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
			
			// Look for duplicate names.
			for (const std::pair<int, Player*>& player : MC::getInstance().getLocalWorld()->getPlayers()) {
				if (player.second->getName() == p->m_name) {
					reason = ConnectionDenialReason::NAME_TAKEN;
					break;
				}
			}

			// Scan name characters.
			int len = strlen(p->m_name);
			for (int i = 0; i < len; ++i) {
				char c = p->m_name[i];
				if (!(isalnum(c) || c == ' ' || c == '-' || c == '_')) {
					reason = ConnectionDenialReason::BAD_NAME;
					break;
				}
			}

			// Send connection response packet.
			Packet<ConnectionResponsePacket> responsePacket;
			responsePacket->m_reason = reason;
			packet_send(client, responsePacket);

			if (reason != ConnectionDenialReason::ALLOWED) {
				server->disconnectPlayer(client, DisconnectReason::UNKNOWN);
				break;
			}

			server->onPlayerConnect(client, p->m_name);
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
