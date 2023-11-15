#pragma once

#include "network.h"
#include "Packet.h"



class MCClientPacketHandler
{
public:

	static void response(nsocket_t socket, const PacketBase& b);
};
