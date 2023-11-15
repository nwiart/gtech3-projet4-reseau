#include "mc/MC.h"
#include "MCServerPacketHandler.h"

#include <string.h>



void MCServerPacketHandler::response(nsocket_t client, const PacketBase& b)
{
	switch (b.m_id)
	{
	case ClientPackets::SetName:
	{
		const Packet<SetNamePacket>& p = *((const Packet<SetNamePacket>*) & b);

		ConnectionDenialReason reason = ConnectionDenialReason::ALLOWED;
		if (strcmp(p->name, "FUCK") == 0) {
			reason = ConnectionDenialReason::BAD_NAME;
		}

		Packet<ConnectionResponsePacket> responsePacket;
		responsePacket->m_reason = reason;
		packet_send(client, responsePacket);

		if (reason != ConnectionDenialReason::ALLOWED) {
			//disconnect_client(client);
		}
	}
	break;

	case ClientPackets::Play:
		break;
	}
}
