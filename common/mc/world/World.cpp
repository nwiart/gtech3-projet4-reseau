#include "mc/MC.h"
#include "World.h"

#include "mc/player/Player.h"

#include "mc/network/server/MCServerClient.h"

#include "mc/world/WorldGenerator.h"



World::World(int sizeX, int sizeY, bool generate)
	: m_sizeX(sizeX), m_sizeY(sizeY), m_tiles(0)
	, m_localPlayer(0)
{
	m_tiles = reinterpret_cast<Tile*>(malloc(sizeX * sizeY * sizeof(Tile)));
	
	// Just zero stuff.
	if (!generate) {
		memset(m_tiles, 0, sizeX * sizeY * sizeof(Tile));
		return;
	}

	// Generate terrain.
	for (int i = 0; i < sizeX * sizeY; ++i) {
		int y = i / sizeX;
		float distCenter = fabs((y / (float) sizeY) * 2.0F - 1.0F);

		m_tiles[i].m_material = 2 - (distCenter * 3.0F - 1.5F >= (rand() / (float)RAND_MAX));
		m_tiles[i].m_broken = distCenter * 3.0F - 2.0F >= (rand() / (float) RAND_MAX);
		m_tiles[i].m_item = 0;
	}

	WorldGenerator gen(this, true);
	gen.generate();
}

World::~World()
{
	// Destroy tiles.
	if (m_tiles) {
		free(m_tiles);
	}

	// Destroy players (including local player).
	for (const std::pair<int, Player*>& pl : m_players) {
		delete pl.second;
	}
}

Player* World::spawnLocalPlayer(int playerID)
{
	extern std::string localPlayerName;

	m_localPlayer = new Player();
	m_localPlayer->teleport(this->m_sizeX / 2, 0);
	m_localPlayer->m_id = playerID;
	m_localPlayer->m_name = localPlayerName;

	m_players.insert(std::pair<int, Player*>(playerID, m_localPlayer));
	m_localPlayer->m_world = this;

	return m_localPlayer;
}

Player* World::spawnRemotePlayer(int playerID, const char* name, int xPos, int yPos)
{
	Player* player = new Player();
	player->teleport(xPos, yPos);
	player->m_id = playerID;
	player->m_name = name;

	m_players.insert(std::pair<int, Player*>(playerID, player));
	player->m_world = this;

	return player;
}

uint16_t World::getTileAt(int x, int y) const
{
	if (this->isOutOfBounds(x, y)) return 0;

	return m_tiles[y * m_sizeX + x].m_material;
}

uint16_t World::getItemAt(int x, int y) const
{
	if (this->isOutOfBounds(x, y)) return 0;

	return m_tiles[y * m_sizeX + x].m_item;
}

bool World::isTileBroken(int x, int y) const
{
	// Make borders solid.
	if (this->isOutOfBounds(x, y)) return false;

	return m_tiles[y * m_sizeX + x].m_broken;
}

bool World::canBreakTile(int x, int y) const
{
	if (this->isOutOfBounds(x, y)) return false;

	return m_tiles[y * m_sizeX + x].m_material == 1;
}

void World::movePlayer(const MCServerClient& player, int dx, int dy)
{
	player.getPlayer()->move(dx, dy);
}

void World::breakTile(int x, int y)
{
	if (x < 0 || x >= m_sizeX || y < 0 || y >= m_sizeY) return;

	m_tiles[y * m_sizeX + x].m_broken = true;
}

void World::despawnPlayer(int playerID)
{
	std::map<int, Player*>::iterator it = m_players.find(playerID);
	if (it == m_players.end()) {
		return;
	}

	Player* player = it->second;

	delete player;

	m_players.erase(it);
}

void World::moveRemotePlayer(int playerID, int xPos, int yPos)
{
	Player* player = m_players.find(playerID)->second;
	if (!player->isRemote()) *((int*) 0) = 0;

	player->teleport(xPos, yPos);
}


void World::getTileRange(uint16_t* tileIDs, int numTiles, int startIndex) const
{
	for (Tile* t = m_tiles + startIndex; t != m_tiles + startIndex + numTiles; t++, tileIDs++) {
		*tileIDs = t->m_material | (t->m_broken << 15);
	}
}

void World::setTileRange(const uint16_t* tileIDs, int numTiles, int startIndex)
{
	for (Tile* t = m_tiles + startIndex; t != m_tiles + startIndex + numTiles; t++, tileIDs++) {
		t->m_material = (*tileIDs & ~(0x8000));
		t->m_broken = (*tileIDs & 0x8000) != 0;
	}
}

void World::setItemAt(uint16_t itemID, int x, int y)
{
	m_tiles[y * m_sizeX + x].m_item = itemID;
}
