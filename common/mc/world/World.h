#pragma once

#include <vector>

class Player;



class World
{
public:

	struct Tile
	{
		uint16_t m_material : 15;
		uint16_t m_broken : 1;
		uint16_t m_item;
	};

public:

	World(int sizeX, int sizeY);
	~World();

	void spawnLocalPlayer(int playerID);

	uint16_t getTileAt(int x, int y) const;
	uint16_t getItemAt(int x, int y) const;
	bool isTileBroken(int x, int y) const;

	void breakTile(int x, int y);

		/// Used by networking to build a world.
	void getTileRange(uint16_t* tileIDs, int numTiles, int startIndex) const;
	void setTileRange(const uint16_t* tileIDs, int numTiles, int startIndex);

	void setItemAt(uint16_t itemID, int x, int y);


	inline const Tile* getTiles() const { return m_tiles; }
	inline int getSizeX() const { return m_sizeX; }
	inline int getSizeY() const { return m_sizeY; }

	inline Player* getLocalPlayer() const { return m_localPlayer; }
	inline const std::vector<Player*>& getPlayers() const { return m_players; }



private:

	Tile* m_tiles;
	int m_sizeX;
	int m_sizeY;

	Player* m_localPlayer;
	std::vector<Player*> m_players;

		/// Is this world hosted by a remote server.
	bool m_isRemote;
};
