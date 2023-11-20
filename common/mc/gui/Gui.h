#pragma once

#include <SFML/Graphics.hpp>



class Gui
{
public:

	Gui() {

	}

	virtual void render(sf::RenderWindow& window) = 0;
};
