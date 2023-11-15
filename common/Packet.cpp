#include "Packet.h"

#include <WinSock2.h>



bool packet_send(nsocket_t socket, const PacketBase& data)
{
	int remaining = sizeof(PacketBase) + data.m_size;
	while (remaining > 0) {
		remaining -= send(socket, (const char*) &data, remaining, 0);
	}

	return true;
}

bool packet_recv(nsocket_t socket, char dest[PACKET_MAX_SIZE])
{
	PacketBase header(0, 0);

	// Read header.
	recv(socket, (char*) &header, sizeof(PacketBase), 0);

	// Read payload.
	recv(socket, dest + sizeof(PacketBase), header.m_size, 0);

	((PacketBase*) dest)->m_id = header.m_id;
	((PacketBase*) dest)->m_size = header.m_size;

	return true;
}
