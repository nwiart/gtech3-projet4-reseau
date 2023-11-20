#include "mc/MC.h"
#include "InputText.h"



InputText::InputText()
{
	m_background.setFillColor(sf::Color::Black);
	m_background.setOutlineColor(sf::Color::White);
	m_background.setOutlineThickness(-1.0F);

	m_renderText.setCharacterSize(12);
	m_renderText.setFont(MC::getInstance().getGlobalFont());
}

void InputText::render(sf::RenderWindow& window)
{
	window.draw(m_background);
	window.draw(m_renderText);
}

void InputText::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);
	m_renderText.setPosition(pos + sf::Vector2f(2.0F, 0.0F));
}

void InputText::setSize(const sf::Vector2f& size)
{
	m_background.setSize(size);
}

void InputText::onTextInput(uint32_t unicodeChar)
{
	m_text += unicodeChar;

	m_renderText.setString(m_text);
}
