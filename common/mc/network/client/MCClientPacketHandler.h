#pragma once

#include "network.h"
#include "Packet.h"



class MCClientPacketHandler
{
public:

	static void handlePacket(nsocket_t socket, const PacketBase& b, void* param);
};
