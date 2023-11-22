#pragma once

#include "mc/gui/Gui.h"

class Inventory;



class GuiInventory : public Gui
{
public:

	GuiInventory(Inventory* inventory);

	virtual void render(sf::RenderWindow& window) override;


private:

	Inventory* m_inventory;
};
