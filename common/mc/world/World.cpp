#include "mc/MC.h"
#include "World.h"



World::World(int sizeX, int sizeY)
	: m_sizeX(sizeX), m_sizeY(sizeY), m_tiles(0)
{
	m_tiles = reinterpret_cast<Tile*>(malloc(sizeX * sizeY * sizeof(Tile)));
	
	for (int i = 0; i < sizeX * sizeY; ++i) {
		m_tiles[i].m_material = 1;
		m_tiles[i].m_item = 0;
	}
}

World::~World()
{
	if (m_tiles) {
		free(m_tiles);
	}
}

uint16_t World::getTileAt(int x, int y) const
{
	if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY) return 0;

	return m_tiles[y * m_sizeX + x].m_material;
}

uint16_t World::getItemAt(int x, int y) const
{
	if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY) return 0;

	return m_tiles[y * m_sizeX + x].m_item;
}

void World::getTileRange(uint16_t* tileIDs, int numTiles, int startIndex) const
{
	for (Tile* t = m_tiles + startIndex; t != m_tiles + startIndex + numTiles; t++, tileIDs++) {
		*tileIDs = t->m_material;
	}
}

void World::setTileRange(const uint16_t* tileIDs, int numTiles, int startIndex)
{
	for (Tile* t = m_tiles + startIndex; t != m_tiles + startIndex + numTiles; t++, tileIDs++) {
		t->m_material = *tileIDs;
	}
}
