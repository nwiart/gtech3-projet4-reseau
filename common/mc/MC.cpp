#include "mc/MC.h"

#include "mc/world/World.h"



MC* MC::m_instance = 0;



MC::MC()
	: m_world(0)
	, m_localPlayer(0)
{
	m_instance = this;
}



void MC::init()
{

}

void MC::openWorld(World* w)
{
	m_world = w;
	m_localPlayer = w->getLocalPlayer();
}
