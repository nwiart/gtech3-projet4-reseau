#pragma once

#include "Gui.h"
#include "CallbackFunc.h"

#include <SFML/Graphics.hpp>

#include <string>



class InputText
{
public:

	typedef void (Gui::*TextInputCallback)();

public:

	InputText();

	void render(sf::RenderWindow& window);

	void setPosition(const sf::Vector2f& pos);
	void setSize(const sf::Vector2f& size);

	inline void setTextInputCallback(Gui* target, TextInputCallback func) { m_callback.set(target, func); }

	template<typename Func>
	inline void setTextInputCallback(Gui* target, Func func) { this->setTextInputCallback(target, static_cast<TextInputCallback>(func)); }

	void setLimit(int l);


	bool onTextInput(uint32_t unicodeChar);

	std::string getText() const { return m_text.toAnsiString(); }

	const sf::Vector2f& getPosition() const { return m_background.getPosition(); }
	const sf::Vector2f& getSize() const { return m_background.getSize(); }


private:

	sf::String m_text;

	sf::RectangleShape m_background;
	sf::Text m_renderText;

	CallbackFunc<TextInputCallback> m_callback;

	int m_limit;
};
