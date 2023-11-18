#include "mc/MC.h"

#include "mc/world/World.h"

#include "mc/registry/TileRegistry.h"



MC* MC::m_instance = 0;



MC::MC()
	: m_world(0)
	, m_localPlayer(0)
{
	m_instance = this;
}



void MC::init()
{
	TileRegistry::init();
}

void MC::openWorld(World* localWorld, WorldServer* serverWorld)
{
	m_localWorld = localWorld;
	m_world = serverWorld;

	m_localPlayer = localWorld ? localWorld->getLocalPlayer() : 0;
}

void MC::openLocalWorld(World* localWorld)
{
	m_localWorld = localWorld;

	m_localPlayer = localWorld->getLocalPlayer();
}
