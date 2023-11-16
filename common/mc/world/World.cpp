#include "mc/MC.h"
#include "World.h"

#include "mc/player/Player.h"



World::World(int sizeX, int sizeY)
	: m_sizeX(sizeX), m_sizeY(sizeY), m_tiles(0)
	, m_localPlayer(0)
{
	m_tiles = reinterpret_cast<Tile*>(malloc(sizeX * sizeY * sizeof(Tile)));
	
	for (int i = 0; i < sizeX * sizeY; ++i) {
		int y = i / sizeX;
		float yp = (y / (float) sizeY);

		m_tiles[i].m_material = 1;
		m_tiles[i].m_broken = fabs(yp - 0.5F)*2.0F*3.0F - 2.0F >= (rand() / (float) RAND_MAX);
		m_tiles[i].m_item = 0;
	}

	// Generate treasure structure.
	int centerX = m_sizeX / 2;
	int centerY = m_sizeY / 2;
	m_tiles[centerY * m_sizeX + centerX].m_broken = true;

	for (int y = -3; y <= 3; ++y) {
		for (int x = -3; x <= 3; ++x) {
			if (x * x + y * y > 10.0F) continue;
			m_tiles[(y + centerY) * m_sizeX + (x + centerX)].m_material = 2;
		}
	}

	this->setItemAt(1, centerX, centerY);

	// Random objects test.
	for (int i = 0; i < 20; ++i) {
		this->setItemAt(1, rand() % m_sizeX, rand() % m_sizeY);
	}
}

World::~World()
{
	if (m_tiles) {
		free(m_tiles);
	}
}

void World::spawnLocalPlayer(int playerID)
{
	m_localPlayer = new Player();
	m_localPlayer->teleport(this->m_sizeX / 2, 0);
	//m_localPlayer->id =;

	m_players.push_back(m_localPlayer);
	m_localPlayer->m_world = this;
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

bool World::isTileBroken(int x, int y) const
{
	// Make borders solid.
	if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY) return false;

	return m_tiles[y * m_sizeX + x].m_broken;
}

void World::breakTile(int x, int y)
{
	if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY) return;

	m_tiles[y * m_sizeX + x].m_broken = true;
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

void World::setItemAt(uint16_t itemID, int x, int y)
{
	m_tiles[y * m_sizeX + x].m_item = itemID;
}
