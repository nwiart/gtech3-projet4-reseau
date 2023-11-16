#pragma once

#include "network.h"
#include "thread.h"

#include "mc/network/server/MCServerClient.h"
#include "DisconnectReason.h"

#include <map>

class MCClient;



class MCServer
{
	friend class MCServerPacketHandler;

public:

		/// Server application's main entry point.
	static int main(bool headless);

	
	MCServer(bool headless);
	~MCServer();

	void host();

	void run();

	void disconnectPlayer(nsocket_t socket, DisconnectReason reason);

		/// Send a packet to a specific client.
	void sendPacket(nsocket_t socket, const PacketBase& b);

		/// Broadcast a packet to all clients at the same time.
	void broadcastPacket(const PacketBase& b, nsocket_t socketExcept = -1);

private:

	void onAccept(nsocket_t socket);
	void onPlayerConnect(nsocket_t socket, const char* name);

	static int adminClientThreadMain(void* param);



private:

		/// Local admin client.
	MCClient* m_adminClient;

		/// Thread running the local client.
	thread m_adminClientThread;

	bool m_headless;

	typedef std::map<nsocket_t, MCServerClient> ClientList;
	typedef std::pair<nsocket_t, MCServerClient> ClientListElement;
	ClientList m_clients;

		/// Player ID increment.
	int m_currentPlayerID;
};
