#pragma once

#include <SFML/Graphics.hpp>

#define TILE_SOLID   (1 << 0)
#define TILE_HARMFUL (1 << 1)



class Tile
{
public:

	Tile() {

	}

	Tile(const sf::Color& color, int flags = TILE_SOLID);
	Tile(const sf::Color& color, const sf::Color& brokenColor, int flags = TILE_SOLID);

	inline const sf::Color getColor() const { return m_tileColor; }
	inline const sf::Color getBrokenColor() const { return m_brokenTileColor; }

	inline bool isSolid() const { return (m_flags & TILE_SOLID) != 0; }
	inline bool isHarmful() const { return (m_flags & TILE_HARMFUL) != 0; }



private:

	sf::Color m_tileColor;
	sf::Color m_brokenTileColor;
	int m_flags;
};



class TileRegistry
{
public:

	static void init();

	static void registerTile(uint16_t id, const Tile& t);

	static const Tile& getTile(uint16_t id) { return m_tiles[id]; }


private:

	static Tile m_tiles[256];
};
