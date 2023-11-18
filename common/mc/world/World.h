#pragma once

#include <map>

class Player;
class MCServerClient;



class World
{
	friend class WorldGenerator;

public:

	struct Tile
	{
		uint16_t m_material : 15;
		uint16_t m_broken : 1;
		uint16_t m_item;
	};

public:

	World(int sizeX, int sizeY, bool generate);
	~World();

	void spawnLocalPlayer(int playerID);
	Player* spawnRemotePlayer(int playerID, int xPos, int yPos);

	uint16_t getTileAt(int x, int y) const;
	uint16_t getItemAt(int x, int y) const;
	bool isTileBroken(int x, int y) const;

	virtual void movePlayer(const MCServerClient& player, int dx, int dy);
	virtual void breakTile(int x, int y);

	virtual void despawnPlayer(int playerID);

	void moveRemotePlayer(int playerID, int xPos, int yPos);


		/// Used by networking to build a world.
	void getTileRange(uint16_t* tileIDs, int numTiles, int startIndex) const;
	void setTileRange(const uint16_t* tileIDs, int numTiles, int startIndex);

	void setItemAt(uint16_t itemID, int x, int y);


	inline const Tile* getTiles() const { return m_tiles; }
	inline int getSizeX() const { return m_sizeX; }
	inline int getSizeY() const { return m_sizeY; }

	inline Player* getLocalPlayer() const { return m_localPlayer; }
	inline const std::map<int, Player*>& getPlayers() const { return m_players; }



private:

	Tile* m_tiles;
	int m_sizeX;
	int m_sizeY;

	Player* m_localPlayer;
	std::map<int, Player*> m_players;

		/// Is this world hosted by a remote server.
	bool m_isRemote;
};
