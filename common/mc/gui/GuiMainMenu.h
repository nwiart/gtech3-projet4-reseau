#pragma once

#include "mc/gui/Gui.h"

#include "mc/gui/InputText.h"



class GuiMainMenu : public Gui
{
public:

	GuiMainMenu();

	virtual void render(sf::RenderWindow& window) override;


private:

	InputText m_inputName;
	InputText m_inputServerIP;
};
