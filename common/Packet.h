#pragma once

#include "network.h"
#include "enum.h"

#include "ConnectionDenialReason.h"
#include "DisconnectReason.h"

#include <stdint.h>


#define PACKET_MAX_SIZE 4096



struct PacketBase
{
	uint16_t m_id;
	uint16_t m_size;

	PacketBase(uint16_t id, uint16_t size)
		: m_id(id), m_size(size)
	{

	}
};

	/// Packet structure to use for sending data.
	/// /!\ Architecture dependent! Byte order is not checked!
template<typename T>
struct Packet : PacketBase
{
	T m_payload;

	Packet()
		: PacketBase(T::ID, sizeof(T))
	{

	}

	T* operator->() { return &m_payload; }
	const T* operator->() const { return &m_payload; }
};

bool packet_send(nsocket_t socket, const PacketBase& data);

bool packet_recv(nsocket_t socket, char dest[PACKET_MAX_SIZE]);



// Client to server.
struct ClientPackets : kdEnum<uint32_t>
{
	enum
	{
		UNKNOWN = 0,
		SetName,            // Sent to provide the server connection info.
		Play,               // Sent when the player made a move.
	};
};

// Server to client.
struct ServerPackets : kdEnum<uint32_t>
{
	enum
	{
		UNKNOWN = 0,
		ConnectionResponse, // Sent to a client to notify whenther their connection was accepted.
		GameWin,            // Sent when a game is won.
		GameRestart,        // Sent when the game restarts.
		Play,               // Sent when someone makes a move.
		Disconnect,
	};
};



// Client packets.

struct SetNamePacket
{
	static const uint32_t ID = ClientPackets::SetName;

	char name[64];
};

struct ClientPlayPacket
{
	static const uint32_t ID = ClientPackets::Play;

	uint16_t m_posX;
	uint16_t m_posY;
};



// Server packets.

struct ConnectionResponsePacket
{
	static const uint32_t ID = ServerPackets::ConnectionResponse;

	ConnectionDenialReason m_reason;
};

struct GameWinPacket
{
	static const uint32_t ID = ServerPackets::GameWin;

	int m_player;
};

struct GameRestartPacket
{
	static const uint32_t ID = ServerPackets::GameRestart;
};

struct ServerPlayPacket
{
	static const uint32_t ID = ServerPackets::Play;

	int m_player;
	uint16_t m_posX;
	uint16_t m_posY;
};

struct ServerDisconnectPacket
{
	static const uint32_t ID = ServerPackets::Disconnect;

	DisconnectReason m_reason;
};
