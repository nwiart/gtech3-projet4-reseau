#include "mc/MC.h"
#include "World.h"



World::World(int sizeX, int sizeY)
	: m_sizeX(sizeX), m_sizeY(sizeY), m_tiles(0)
{
	m_tiles = reinterpret_cast<Tile*>(malloc(sizeX * sizeY * sizeof(Tile)));
}

World::~World()
{
	if (m_tiles) {
		free(m_tiles);
	}
}



