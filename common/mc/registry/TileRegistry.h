#pragma once

#include <SFML/Graphics.hpp>



class Tile
{
public:

	Tile()
	{

	}


private:

	sf::Color m_tileColor;
	sf::Color m_brokenTileColor;
	bool m_solid;
};



class TileRegistry
{
public:

	static void registerTile(uint16_t id, const Tile& t);


private:

	Tile m_tiles[256];
};
