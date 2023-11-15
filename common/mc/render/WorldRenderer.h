#pragma once

#include <SFML/Graphics.hpp>

class World;



class WorldRenderer
{
public:

	static void render(sf::RenderWindow& window, World* world);
};
