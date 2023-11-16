#pragma once

#include "network.h"

class Player;



class MCServerClient
{
	friend class MCServer;

private:

	MCServerClient(nsocket_t socket)
		: m_socket(socket)
		, m_player(0)
		, m_playerID(-1)
	{

	}

public:

	inline nsocket_t getSocket() const { return m_socket; }

	inline int getPlayerID() const { return m_playerID; }
	inline Player* getPlayer() const { return m_player; }


private:

	nsocket_t m_socket;

	int m_playerID;
	Player* m_player;
};
