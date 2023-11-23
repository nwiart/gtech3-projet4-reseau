#include "mc/MC.h"
#include "GuiStatus.h"

#include "mc/gui/GuiMainMenu.h"

#include "mc/network/client/MCClient.h"

#include <iostream>

#include <WinSock2.h>
#include <Windows.h>



GuiStatus* GuiStatus::m_instance = 0;

std::string GuiStatus::m_status;
std::string GuiStatus::m_desc;
bool        GuiStatus::m_showBackButton = false;



void GuiStatus::setStatus(const char* str, const char* desc, bool showBackButton)
{
	m_status = str ? str : "";
	m_desc = desc ? desc : "";
	m_showBackButton = showBackButton;
}

void GuiStatus::displayStatus()
{
	CRITICAL_SECTION s;
	InitializeCriticalSection(&s);

	EnterCriticalSection(&s);
	MCClient::getInstance().m_showStatus = true;
	LeaveCriticalSection(&s);

	DeleteCriticalSection(&s);
}


GuiStatus::GuiStatus()
{
	m_instance = this;
	

	m_textStatus.setPosition(sf::Vector2f(0.0F, 120.0F));
	m_textDesc.setPosition(sf::Vector2f(0.0F, 160.0F));

	m_buttonBack.setSize(sf::Vector2f(180, 12+5));
	m_buttonBack.setPosition(sf::Vector2f((320 - 180) * 0.5F, 240));
	m_buttonBack.setText("Back");
	m_buttonBack.setCallback(this, &GuiStatus::onButtonBack);

	m_texts.push_back(&m_textStatus);
	m_texts.push_back(&m_textDesc);
	m_buttons.push_back(&m_buttonBack);


	m_textStatus.setText(m_status.c_str());
	m_textDesc.setText(m_desc.c_str());
	m_textStatus.setCenteredH();
	m_textDesc.setCenteredH();

	m_buttonBack.setVisible(m_showBackButton);
}

GuiStatus::~GuiStatus()
{
	m_instance = 0;
}

void GuiStatus::render(sf::RenderWindow& window)
{
	sf::RectangleShape r(sf::Vector2f(320.0F, 320.0F));
	r.setFillColor(sf::Color(70, 50, 30));
	window.draw(r);

	Gui::render(window);
}

void GuiStatus::onButtonBack()
{
	GuiMainMenu* menu = new GuiMainMenu();
	MCClient::getInstance().openGui(menu);
}
