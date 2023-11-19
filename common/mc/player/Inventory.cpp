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
		m_items.insert(std::pair<uint16_t, int>(itemID, amount));
	}
	else {
		it->second += amount;
	}
}

bool Inventory::removeItem(uint16_t itemID, int amount)
{
	if (itemID == 0) return true;

	auto it = m_items.find(itemID);
	if (it == m_items.end() || it->second < amount) {
		return false;
	}

	if (it->second == amount) {
		m_items.erase(it);
	}
	else {
		it->second -= amount;
	}

	return true;
}

int Inventory::getItemAmountByID(uint16_t itemID) const
{
	auto it = m_items.find(itemID);

	return (it == m_items.end()) ? 0 : it->second;
}
