#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class Text;
class Button;
class InputText;



class Gui
{
public:

	Gui();

	virtual void render(sf::RenderWindow& window);

	void onLeftClick(int x, int y);

	void onTextInput(uint32_t unicodeChar);



protected:

	InputText* m_focusedInputText;

	std::vector<Text*> m_texts;
	std::vector<Button*> m_buttons;
	std::vector<InputText*> m_inputTexts;
};
