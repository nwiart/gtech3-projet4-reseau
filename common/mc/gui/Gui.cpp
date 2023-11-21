#include "mc/MC.h"
#include "Gui.h"

#include "mc/gui/Text.h"
#include "mc/gui/Button.h"
#include "mc/gui/InputText.h"



Gui::Gui()
	: m_focusedInputText(0)
{

}

void Gui::render(sf::RenderWindow& window)
{
	for (InputText* input : m_inputTexts) {
		input->render(window);
	}

	for (Button* button : m_buttons) {
		button->render(window);
	}

	for (Text* text : m_texts) {
		text->render(window);
	}
}

void Gui::onLeftClick(int x, int y)
{
	m_focusedInputText = 0;

	// Transform coords.
	x /= 2;
	y /= 2;

	for (InputText* input : m_inputTexts) {
		const sf::Vector2f& b = input->getPosition();
		const sf::Vector2f& e = b + input->getSize();

		if (x >= b.x && x < e.x && y >= b.y && y < e.y) {
			m_focusedInputText = input;
			break;
		}
	}

	for (Button* button : m_buttons) {
		const sf::Vector2f& b = button->getPosition();
		const sf::Vector2f& e = b + button->getSize();

		if (x >= b.x && x < e.x && y >= b.y && y < e.y) {
			button->onLeftClick();
			break;
		}
	}
}

void Gui::onTextInput(uint32_t unicodeChar)
{
	if (!m_focusedInputText) return;

	if (m_focusedInputText->onTextInput(unicodeChar)) {
		m_focusedInputText = 0;
	}
}
