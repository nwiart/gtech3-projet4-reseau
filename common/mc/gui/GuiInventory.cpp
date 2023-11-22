#include "mc/MC.h"
#include "GuiInventory.h"

#include "mc/gui/ItemSlot.h"

#include "mc/player/Inventory.h"



GuiInventory::GuiInventory(Inventory* inventory)
	: m_inventory(inventory)
{

}

void GuiInventory::render(sf::RenderWindow& window)
{
	int posY = 0;
	for (std::pair<const uint16_t, ItemStack>& p : m_inventory->getItems()) {
		ItemSlot s(&p.second);
		s.setPosition(sf::Vector2f(320-20, posY));
		s.render(window);

		posY += 20;
	}
}
