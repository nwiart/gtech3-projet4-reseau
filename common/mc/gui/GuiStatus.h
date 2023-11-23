#pragma once

#include "Gui.h"

#include "mc/gui/Text.h"
#include "mc/gui/Button.h"

#include <string>



	/// Server connection status screen.
class GuiStatus : public Gui
{
	/// Global interface.
public:

	static GuiStatus* m_instance;

	static void setStatus(const char* str, const char* desc, bool showBackButton);
	static void displayStatus();


public:

	GuiStatus();
	virtual ~GuiStatus();

	virtual void render(sf::RenderWindow& window) override;

private:

	void onButtonBack();



private:

	Text m_textStatus;
	Text m_textDesc;
	Button m_buttonBack;

	static std::string m_status;
	static std::string m_desc;
	static bool m_showBackButton;
};
