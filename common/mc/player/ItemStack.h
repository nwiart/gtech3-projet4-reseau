#pragma once



class ItemStack
{
public:

	ItemStack();
	ItemStack(uint16_t item, uint16_t amount = 1, uint16_t damage = 0);

	void addAmount(int c);
	bool removeAmount(int c);

	inline bool isEmpty() const { return m_amount == 0; }

	inline uint16_t getItemID() const { return m_itemID; }
	inline uint16_t getDamage() const { return m_damage; }
	inline uint16_t getAmount() const { return m_amount; }


private:

	uint16_t m_itemID;
	uint16_t m_damage;
	uint16_t m_amount;
};
