#pragma once

#include <stdlib.h>
#include <stdint.h>

class World;
class Player;



class MC
{
public:

	static inline MC& getInstance() { return *m_instance; }

	MC();

	void init();

	void openWorld(World* w);

	inline World* getWorld() { return m_world; }
	inline Player* getLocalPlayer() { return m_localPlayer; }

public:

	static const uint16_t SERVER_PORT = 27015;
	static const uint16_t WEB_PORT    = SERVER_PORT + 1;



private:

	static MC* m_instance;

	World* m_world;
	Player* m_localPlayer;
};
