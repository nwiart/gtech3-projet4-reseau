#pragma once

#include "Gui.h"
#include "CallbackFunc.h"

#include <SFML/Graphics.hpp>

#include <string>



class Button
{
public:

	typedef void (Gui::*Callback)();

public:

	Button();

	void render(sf::RenderWindow& window);

	void onLeftClick();

	void setPosition(const sf::Vector2f& pos);
	void setSize(const sf::Vector2f& size);

	void setText(const char* str);
	void setVisible(bool v);
	void setEnabled(bool b);

	inline void setCallback(Gui* target, Callback func) { m_callback.set(target, func); }

	template<typename Func>
	inline void setCallback(Gui* target, Func func) { this->setCallback(target, static_cast<Callback>(func)); }


	inline const sf::Vector2f& getPosition() const { return m_background.getPosition(); }
	inline const sf::Vector2f& getSize() const { return m_background.getSize(); }

	inline bool isVisible() const { return m_visible; }

private:

	void updateTextPosition();


private:

	CallbackFunc<Callback> m_callback;

	sf::RectangleShape m_background;
	sf::Text m_renderText;

	bool m_visible;
	bool m_enabled;
};
