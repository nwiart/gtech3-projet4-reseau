#include "mc/MC.h"
#include "GuiMainMenu.h"

#include "mc/network/client/MCClient.h"



GuiMainMenu::GuiMainMenu()
{
	// Sizes.
	m_inputName.setSize(sf::Vector2f(240, 12 + 5));
	m_inputServerIP.setSize(sf::Vector2f(240, 12 + 5));
	m_buttonJoinServer.setSize(sf::Vector2f(180, 12 + 5));

	// Positions.
	m_inputName.setPosition(sf::Vector2f((320.0F - m_inputName.getSize().x) * 0.5F, 120));
	m_inputServerIP.setPosition(sf::Vector2f((320.0F - m_inputServerIP.getSize().x) * 0.5F, 180));
	m_buttonJoinServer.setPosition(sf::Vector2f((320.0F - m_buttonJoinServer.getSize().x) * 0.5F, 260));


	m_textName.setPosition(m_inputName.getPosition() + sf::Vector2f(0.0F, -20.0F));
	m_textServerIP.setPosition(m_inputServerIP.getPosition() + sf::Vector2f(0.0F, -20.0F));


	m_textName.setText("Enter name :");
	m_textServerIP.setText("Enter server IP :");
	m_buttonJoinServer.setText("Join server");
	m_buttonJoinServer.setCallback(this, &GuiMainMenu::onJoinServer);
	m_buttonJoinServer.setEnabled(false);

	m_inputName.setTextInputCallback(this, &GuiMainMenu::onInput);
	m_inputServerIP.setTextInputCallback(this, &GuiMainMenu::onInput);


	// Register elements.
	m_texts.push_back(&m_textName);
	m_texts.push_back(&m_textServerIP);

	m_buttons.push_back(&m_buttonJoinServer);

	m_inputTexts.push_back(&m_inputName);
	m_inputTexts.push_back(&m_inputServerIP);
}

void GuiMainMenu::render(sf::RenderWindow& window)
{
	sf::RectangleShape r(sf::Vector2f(320.0F, 320.0F));
	r.setFillColor(sf::Color(70, 50, 30));
	window.draw(r);

	Gui::render(window);
}


void GuiMainMenu::onJoinServer()
{
	extern std::string localPlayerName;
	localPlayerName = m_inputName.getText();

	MCClient::getInstance().connect(m_inputServerIP.getText().c_str(), MC::SERVER_PORT);

	MCClient::getInstance().openGui(0);
}

void GuiMainMenu::onInput()
{
	m_buttonJoinServer.setEnabled(!m_inputName.getText().empty() && !m_inputServerIP.getText().empty());
}
