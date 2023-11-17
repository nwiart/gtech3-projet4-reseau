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
		Join,               // Sent to provide the server connection info.
		PlayerMove,         // Sent when the player made a move.
	};
};

// Server to client.
struct ServerPackets : kdEnum<uint32_t>
{
	enum
	{
		UNKNOWN = 0,
		ConnectionResponse, // Sent to a client to notify whenther their connection was accepted.
		GetWorldDimensions, // Sent to notify clients of the world size.
		GetWorldTiles,      // Sent to serve tile data to the game (without items).
		GetPlayerID,
		PlayerSpawn,
		GameWin,            // Sent when a game is won.
		GameRestart,        // Sent when the game restarts.
		PlayerMove,         // Sent when someone makes a move.
		BlockBreak,
		Disconnect,
	};
};



//
// Client packets.
//

struct ClientJoinPacket
{
	static const uint32_t ID = ClientPackets::Join;

		/// Player name.
	char m_name[64];

		/// Joining as a spectator.
	bool m_isSpectator;
};

struct ClientPlayerMovePacket
{
	static const uint32_t ID = ClientPackets::PlayerMove;

		/// Movement delta.
	int16_t m_dx;
	int16_t m_dy;
};



//
// Server packets.
//

struct ConnectionResponsePacket
{
	static const uint32_t ID = ServerPackets::ConnectionResponse;

	ConnectionDenialReason m_reason;
};

struct ServerGetWorldDimensionsPacket
{
	static const uint32_t ID = ServerPackets::GetWorldDimensions;

	uint32_t m_sizeX;
	uint32_t m_sizeY;
};

struct ServerGetWorldTilesPacket
{
	static const uint32_t ID = ServerPackets::GetWorldTiles;

	uint32_t m_startIndex;
	uint32_t m_numTiles;
	uint16_t m_tiles[256];  // Tiles buffer. Highest bit is the block state (broken / still present).
};

struct ServerGetPlayerIDPacket
{
	static const uint32_t ID = ServerPackets::GetPlayerID;

	int m_playerID;
};

struct ServerPlayerSpawnPacket
{
	static const uint32_t ID = ServerPackets::PlayerSpawn;

	uint32_t m_playerID;
	uint16_t m_xPos;
	uint16_t m_yPos;
	char m_playerName[64];
};

struct GameWinPacket
{
	static const uint32_t ID = ServerPackets::GameWin;

	int m_playerID;
};

struct GameRestartPacket
{
	static const uint32_t ID = ServerPackets::GameRestart;
};

struct ServerPlayerMovePacket
{
	static const uint32_t ID = ServerPackets::PlayerMove;

		/// Player ID of the moved player.
	int m_playerID;

		/// Player's absolute position.
	int16_t m_posX;
	int16_t m_posY;
};

struct ServerBlockBreakPacket
{
	static const uint32_t ID = ServerPackets::BlockBreak;

		/// Block position.
	uint16_t m_posX;
	uint16_t m_posY;

		/// Optional item revealed inside the block. Set to 0 for none.
	uint16_t m_item;
};

struct ServerDisconnectPacket
{
	static const uint32_t ID = ServerPackets::Disconnect;

	int m_playerID;
	DisconnectReason m_reason;
};
