#pragma once

#include <SFML/Graphics.hpp>



class Item
{
public:

	Item() {

	}

	Item(const sf::Vector2f& size);



private:

	sf::RectangleShape m_sfRectangle;
};



class ItemRegistry
{
public:

	static void init();

	static void registerItem(uint16_t id, const Item& item);

	static const Item& getItem(uint16_t id) { return m_items[id]; }


private:

	static Item m_items[256];
};
