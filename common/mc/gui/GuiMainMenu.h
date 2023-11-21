#pragma once

#include "mc/gui/Gui.h"

#include "mc/gui/Text.h"
#include "mc/gui/Button.h"
#include "mc/gui/InputText.h"



class GuiMainMenu : public Gui
{
public:

	GuiMainMenu();

	virtual void render(sf::RenderWindow& window) override;

private:

	void onJoinServer();
	void onInput();


private:

	Text m_textName;
	Text m_textServerIP;

	Button m_buttonJoinServer;

	InputText m_inputName;
	InputText m_inputServerIP;
};
