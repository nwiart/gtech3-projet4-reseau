#include "mc/MC.h"
#include "InputText.h"

#include "mc/network/client/MCClient.h"



InputText::InputText()
	: m_limit(256)
{
	m_background.setFillColor(sf::Color::Black);
	m_background.setOutlineColor(sf::Color::White);
	m_background.setOutlineThickness(-1.0F);

	m_renderText.setCharacterSize(12);
	m_renderText.setFont(MCClient::getInstance().getGlobalFont());
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

void InputText::setLimit(int l)
{
	m_limit = l;
}


bool InputText::onTextInput(uint32_t unicodeChar)
{
	switch (unicodeChar) {
	case '\b':
		if (!m_text.isEmpty()) {
			m_text.erase(m_text.getSize() - 1);
		}
		break;

	case '\n':
	case '\r':
		return true;

	default:
		if (m_text.getSize() != m_limit) {
			m_text += unicodeChar;
		}
		break;
	}

	m_renderText.setString(m_text);

	m_callback.invoke();

	return false;
}
