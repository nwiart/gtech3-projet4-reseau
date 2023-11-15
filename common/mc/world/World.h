#pragma once



class World
{
public:

	World(int sizeX, int sizeY);
	~World();

	uint16_t getTileAt(int x, int y) const;
	uint16_t getItemAt(int x, int y) const;

		/// Used by networking to build a world.
	void getTileRange(uint16_t* tileIDs, int numTiles, int startIndex) const;
	void setTileRange(const uint16_t* tileIDs, int numTiles, int startIndex);



private:

	struct Tile
	{
		uint16_t m_material;
		uint16_t m_item;
	};

	Tile* m_tiles;
	int m_sizeX;
	int m_sizeY;
};
