#pragma once

#include "network.h"
#include "Packet.h"



class MCServerPacketHandler
{
public:

	static void handleAccept(nsocket_t socket, void* param);

	static void handleClose(nsocket_t socket, void* param);

	static void handlePacket(nsocket_t socket, const PacketBase& b, void* param);
};
