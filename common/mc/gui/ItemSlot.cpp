#include "mc/MC.h"
#include "ItemSlot.h"

#include "mc/player/ItemStack.h"

#include "mc/network/client/MCClient.h"



ItemSlot::ItemSlot(ItemStack* itemStack)
	: m_itemStack(itemStack)
{
	m_background.setSize(sf::Vector2f(20.0F, 20.0F));
	m_background.setFillColor(sf::Color::Black);
	m_background.setOutlineColor(sf::Color::White);
	m_background.setOutlineThickness(-1.0F);

	m_textAmount.setCharacterSize(12);
	m_textAmount.setFont(MCClient::getInstance().getGlobalFont());
}

void ItemSlot::render(sf::RenderWindow& window)
{
	int amount = m_itemStack ? m_itemStack->getAmount() : 0;

	window.draw(m_background);

	if (amount > 1) {
		m_textAmount.setString(std::to_string(amount));
		window.draw(m_textAmount);
	}
}

void ItemSlot::onLeftClick()
{
	
}

void ItemSlot::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);
	m_textAmount.setPosition(pos + sf::Vector2f(1.0F, 6.0F));
}
