#pragma once

#include "mc/world/World.h"

class MCServer;



	/// Subclass of World, responding to world events with packet sending.
	/// This world class is used specially for storing a server-side world.
class WorldServer : private World
{
	friend class MCServer;

public:

	WorldServer(int sizeX, int sizeY, MCServer* server);

		/// Moves a player, and determines the consequences, so that the server sends
		/// appropriate packets.
	virtual void movePlayer(const MCServerClient& player, int dx, int dy) override;

	virtual void breakTile(int x, int y) override;


private:

		/// The server controlling this world.
	MCServer* m_server;
};
