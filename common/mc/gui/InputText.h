#pragma once

#include <SFML/Graphics.hpp>



class InputText
{
public:

	InputText();

	void render(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& pos);
	void setSize(const sf::Vector2f& size);

	void onTextInput(uint32_t unicodeChar);

	const sf::String& getText() const { return m_text; }

	const sf::Vector2f& getPosition() const { return m_background.getPosition(); }
	const sf::Vector2f& getSize() const { return m_background.getSize(); }


private:

	sf::String m_text;

	sf::RectangleShape m_background;
	sf::Text m_renderText;
};
