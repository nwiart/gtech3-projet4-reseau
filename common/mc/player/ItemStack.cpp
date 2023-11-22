#include "mc/MC.h"
#include "ItemStack.h"



ItemStack::ItemStack()
	: m_itemID(0)
	, m_amount(0)
	, m_damage(0)
{

}

ItemStack::ItemStack(uint16_t item, uint16_t amount, uint16_t damage)
	: m_itemID(item)
	, m_amount(amount)
	, m_damage(damage)
{

}

void ItemStack::addAmount(int c)
{
	m_amount += c;
}

bool ItemStack::removeAmount(int c)
{
	if (m_amount < c) return false;

	m_amount -= c;
	if (m_amount == 0) m_itemID = 0;

	return true;
}
