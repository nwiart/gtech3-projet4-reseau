#pragma once

#include "ItemStack.h"

#include <map>



	/// Player inventory.
class Inventory
{
public:

	Inventory();

	void addItem(uint16_t itemID, int amount);

	bool removeItem(uint16_t itemID, int amount);

	const ItemStack* getItemStackByID(uint16_t itemID) const;
	ItemStack* getItemStackByID(uint16_t itemID);

	inline const std::map<uint16_t, ItemStack>& getItems() const { return m_items; }
	inline std::map<uint16_t, ItemStack>& getItems() { return m_items; }


private:

		/// Items.
	std::map<uint16_t, ItemStack> m_items;
};
