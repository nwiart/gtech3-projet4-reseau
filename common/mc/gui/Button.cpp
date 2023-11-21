#include "mc/MC.h"
#include "Button.h"

#include "mc/network/client/MCClient.h"



Button::Button()
	: m_enabled(false)
{
	m_background.setOutlineColor(sf::Color::Black);
	m_background.setOutlineThickness(-1.0F);

	m_renderText.setCharacterSize(12);
	m_renderText.setFont(MCClient::getInstance().getGlobalFont());

	this->setEnabled(true);
}

void Button::render(sf::RenderWindow& window)
{
	window.draw(m_background);

	sf::Text shadow(m_renderText);
	shadow.move(sf::Vector2f(1.0F, 1.0F));
	shadow.setFillColor(sf::Color(60, 60, 60));

	window.draw(shadow);
	window.draw(m_renderText);
}

void Button::onLeftClick()
{
	if (m_enabled) {
		m_callback.invoke();
	}
}

void Button::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);
	this->updateTextPosition();
}

void Button::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);
	this->updateTextPosition();
}

void Button::setText(const char* str)
{
	m_renderText.setString(str);
	this->updateTextPosition();
}

void Button::setEnabled(bool b)
{
	m_enabled = b;

	m_background.setFillColor(b ? sf::Color(140, 140, 140) : sf::Color(70, 70, 70));
	m_renderText.setFillColor(b ? sf::Color(255, 255, 255) : sf::Color(160, 160, 160));
}


void Button::updateTextPosition()
{
	m_renderText.setPosition(m_background.getPosition() + sf::Vector2f((m_background.getSize().x - m_renderText.getLocalBounds().getSize().x) * 0.5F, 0.0F));
}
