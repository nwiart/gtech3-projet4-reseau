#include "mc/MC.h"
#include "Text.h"

#include "mc/network/client/MCClient.h"



Text::Text()
{
	m_renderText.setString("Text");
	m_renderText.setCharacterSize(12);
	m_renderText.setFont(MCClient::getInstance().getGlobalFont());
}

void Text::render(sf::RenderWindow& window)
{
	sf::Text shadow(m_renderText);
	shadow.move(sf::Vector2f(1.0F, 1.0F));
	shadow.setFillColor(sf::Color(60, 60, 60));

	window.draw(shadow);
	window.draw(m_renderText);
}

void Text::setPosition(const sf::Vector2f& pos)
{
	m_renderText.setPosition(pos);
}

void Text::setText(const char* str)
{
	m_renderText.setString(str);
}
