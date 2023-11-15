#pragma once

#include "network.h"
#include "Packet.h"



class MCServerPacketHandler
{
public:

	static void response(nsocket_t socket, const PacketBase& b);
};
