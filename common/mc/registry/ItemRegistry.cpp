#include "mc/MC.h"
#include "ItemRegistry.h"



Item ItemRegistry::m_items[256];



Item::Item(const sf::Vector2f& size)
{
	m_sfRectangle.setSize(size * (1.0F / 16.0F));
	m_sfRectangle.setOrigin(m_sfRectangle.getSize() * 0.5F);
}



void ItemRegistry::init()
{
	registerItem(1, Item());
}

void ItemRegistry::registerItem(uint16_t id, const Item& item)
{
	m_items[id] = item;
}
