#include "mc/MC.h"
#include "Player.h"

#include "mc/world/World.h"

#include "mc/network/client/MCClient.h"
#include "Packet.h"



Player::Player()
	: m_xPos(0), m_yPos(0), m_world(0)
{

}

void Player::move(int dx, int dy)
{
	if (dx && dy) return;
	if (dx < -1 || dx > 1 || dy < -1 || dy > 1) return;

	if (dx) {
		uint16_t tileID = m_world->getTileAt(m_xPos + dx, m_yPos);
		if (tileID == 0) {
			m_xPos += dx;
		}
	}
	else if (dy) {
		uint16_t tileID = m_world->getTileAt(m_xPos, m_yPos + dy);
		if (tileID == 0) {
			m_yPos += dy;
		}
	}

	if (!this->isRemote()) {
		Packet<ClientPlayerMovePacket> p;
		p->m_dx = dx;
		p->m_dy = dy;
		MCClient::getInstance().sendPacket(p);
	}
}

bool Player::isRemote() const
{
	return MC::getInstance().getLocalPlayer() == this;
}
