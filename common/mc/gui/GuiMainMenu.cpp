#include "mc/MC.h"
#include "GuiMainMenu.h"



GuiMainMenu::GuiMainMenu()
{
	m_inputName.setSize(sf::Vector2f(240, 12 + 5));
	m_inputServerIP.setSize(sf::Vector2f(240, 12 + 5));

	m_inputName.setPosition(sf::Vector2f((320.0F - m_inputName.getSize().x) * 0.5F, 80));
	m_inputServerIP.setPosition(sf::Vector2f((320.0F - m_inputServerIP.getSize().x) * 0.5F, 120));
}

void GuiMainMenu::render(sf::RenderWindow& window)
{
	m_inputName.render(window);
	m_inputServerIP.render(window);
}
