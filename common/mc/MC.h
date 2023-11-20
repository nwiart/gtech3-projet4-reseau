#pragma once

#include <stdlib.h>
#include <stdint.h>

namespace sf { class Font; }

class World;
class WorldServer;

class Player;



class MC
{
public:

	static inline MC& getInstance() { return *m_instance; }

	MC();

	void init();

		/// Server world should be non-null only if this instance is hosting.
	void openWorld(World* localWorld, WorldServer* serverWorld);
	void openLocalWorld(World* localWorld);

	inline World* getLocalWorld() { return m_localWorld; }
	inline WorldServer* getWorld() { return m_world; }
	inline Player* getLocalPlayer() { return m_localPlayer; }

	inline const sf::Font& getGlobalFont() const { return *m_font; }


public:

	static const uint16_t SERVER_PORT = 27015;
	static const uint16_t WEB_PORT    = SERVER_PORT + 1;



private:

	static MC* m_instance;

	World* m_localWorld;
	WorldServer* m_world;

	Player* m_localPlayer;


	sf::Font* m_font;
};
