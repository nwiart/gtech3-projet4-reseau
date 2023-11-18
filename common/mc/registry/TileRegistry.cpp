#include "mc/MC.h"
#include "TileRegistry.h"



Tile TileRegistry::m_tiles[256];

static const sf::Color DARK(140, 140, 140);



Tile::Tile(const sf::Color& color, int flags)
	: m_tileColor(color), m_brokenTileColor(color * DARK), m_flags(flags)
{

}

Tile::Tile(const sf::Color& color, const sf::Color& brokenColor, int flags)
	: m_tileColor(color), m_brokenTileColor(brokenColor), m_flags(flags)
{

}



void TileRegistry::init()
{
	// Dirt, stone and bedrock.
	registerTile(1, Tile(sf::Color(170, 110, 60)));
	registerTile(2, Tile(sf::Color(127, 127, 127)));
	registerTile(3, Tile(sf::Color(80,  80,  80)));

	// Lava.
	registerTile(4, Tile(sf::Color(127, 127, 127), sf::Color(255, 20, 0), TILE_SOLID | TILE_HARMFUL));
}

void TileRegistry::registerTile(uint16_t id, const Tile& t)
{
	m_tiles[id] = t;
}
