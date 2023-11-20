#include "mc/MC.h"
#include "Player.h"

#include "mc/world/World.h"
#include "mc/player/Inventory.h"

#include "mc/network/client/MCClient.h"
#include "Packet.h"



Player::Player()
	: m_xPos(0), m_yPos(0), m_world(0)
{
	m_inventory = new Inventory();
}

Player::~Player()
{
	if (m_inventory) {
		delete m_inventory;
	}
}

void Player::move(int dx, int dy)
{
	if (dx && dy) return;
	if (dx < -1 || dx > 1 || dy < -1 || dy > 1) return;

	if (dx) {
		if (m_world->isTileBroken(m_xPos + dx, m_yPos)) {
			m_xPos += dx;
		}
		else if (m_world->canBreakTile(m_xPos + dx, m_yPos)) {
			m_world->breakTile(m_xPos + dx, m_yPos);
		}
	}
	else if (dy) {
		if (m_world->isTileBroken(m_xPos, m_yPos + dy)) {
			m_yPos += dy;
		}
		else if (m_world->canBreakTile(m_xPos, m_yPos + dy)) {
			m_world->breakTile(m_xPos, m_yPos + dy);
		}
	}

	if (!this->isRemote()) {
		Packet<ClientPlayerMovePacket> p;
		p->m_dx = dx;
		p->m_dy = dy;
		MCClient::getInstance().sendPacket(p);
	}
}

void Player::teleport(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
}

bool Player::isRemote() const
{
	return MC::getInstance().getLocalPlayer() != this;
}
