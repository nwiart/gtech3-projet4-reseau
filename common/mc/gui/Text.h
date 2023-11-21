#pragma once

#include <SFML/Graphics.hpp>

#include <string>



class Text
{
public:

	Text();

	void render(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& pos);

	void setText(const char* str);



private:

	sf::Text m_renderText;
};
