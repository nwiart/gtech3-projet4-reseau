#pragma once

#include "network.h"
#include "thread.h"

#include "mc/network/server/MCServerClient.h"
#include "DisconnectReason.h"

#include <map>
#include <unordered_map>
#include <string>

class MCClient;



class MCServer
{
	friend class MCServerPacketHandler;

public:

	typedef std::map<nsocket_t, MCServerClient> ClientList;
	typedef std::pair<nsocket_t, MCServerClient> ClientListElement;

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

	const MCServerClient& getClient(nsocket_t socket) const;

	inline const ClientList& getClients() const { return m_clients; }

private:

	MCServerClient& getClient(nsocket_t socket);

		/// New sockets and closing sockets handlers.
	void onAccept(nsocket_t socket);
	void onClose(nsocket_t socket);

		/// Player finalized their connection.
	void onPlayerConnect(nsocket_t socket, const char* name);

	void onPlayerMove(nsocket_t socket, int dx, int dy);


	static int adminClientThreadMain(void* param);



private:

		/// Local admin client.
	MCClient* m_adminClient;

		/// Thread running the local client.
	thread m_adminClientThread;

	bool m_headless;

	ClientList m_clients;

		/// Player ID increment.
	int m_currentPlayerID;


	std::map<std::string, std::string> m_serverConfig;
};
