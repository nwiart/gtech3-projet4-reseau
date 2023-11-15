#pragma once

#include <stdlib.h>
#include <stdint.h>

class World;
class Player;



class MC
{
public:

	static void init();

	static inline World* getWorld() { return m_world; }
	static inline Player* getLocalPlayer() { return m_localPlayer; }

	static const uint16_t SERVER_PORT = 27015;
	static const uint16_t WEB_PORT    = SERVER_PORT + 1;


private:

	static World* m_world;
	static Player* m_localPlayer;
};
