#pragma once



class World
{
public:

	World(int sizeX, int sizeY);
	~World();



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
