#pragma once

#include <map>



	/// Player inventory.
class Inventory
{
public:

	Inventory();

	void addItem(uint16_t itemID, int amount);

	bool removeItem(uint16_t itemID, int amount);

	int getItemAmountByID(uint16_t itemID) const;

	inline const std::map<uint16_t, int>& getStackableItems() const { return m_items; }


private:

		/// Stackable items.
	std::map<uint16_t, int> m_items;
};
