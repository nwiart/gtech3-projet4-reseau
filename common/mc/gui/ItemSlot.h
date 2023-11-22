#pragma once

#include <SFML/Graphics.hpp>

class ItemStack;



class ItemSlot
{
public:

	ItemSlot(ItemStack* itemStack);

	void render(sf::RenderWindow& window);

	void onLeftClick();

	void setPosition(const sf::Vector2f& pos);


	const sf::Vector2f& getPosition() const { return m_background.getPosition(); }
	const sf::Vector2f& getSize() const { return m_background.getSize(); }


private:

	ItemStack* m_itemStack;

	sf::RectangleShape m_background;
	sf::Text m_textAmount;
};
