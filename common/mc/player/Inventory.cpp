#include "mc/MC.h"
#include "Inventory.h"



Inventory::Inventory()
{

}

void Inventory::addItem(uint16_t itemID, int amount)
{
	if (itemID == 0) return;

	auto it = m_items.find(itemID);

	if (it == m_items.end()) {
		ItemStack is(itemID, amount, 0);
		m_items.insert(std::pair<uint16_t, ItemStack>(itemID, is));
	}
	else {
		it->second.addAmount(amount);
	}
}

bool Inventory::removeItem(uint16_t itemID, int amount)
{
	if (itemID == 0) return true;

	auto it = m_items.find(itemID);
	if (it == m_items.end()) {
		return false;
	}

	if (it->second.getAmount() == amount) {
		m_items.erase(it);
		return true;
	}

	return it->second.removeAmount(amount);
}

const ItemStack* Inventory::getItemStackByID(uint16_t itemID) const
{
	auto it = m_items.find(itemID);

	return (it == m_items.end()) ? 0 : &it->second;
}

ItemStack* Inventory::getItemStackByID(uint16_t itemID)
{
	auto it = m_items.find(itemID);

	return (it == m_items.end()) ? 0 : &it->second;
}
