#pragma once

#include "network.h"
#include "thread.h"

class MCClient;



class MCServer
{
public:

		/// Server application's main entry point.
	static int main(bool headless);

	
	MCServer(bool headless);
	~MCServer();

	void host();

	void run();

		/// Send a packet to a specific client.
	void sendPacket(??, const PacketBase& b);

		/// Broadcast a packet to all clients at the same time.
	void broadcastPacket(const PacketBase& b);

private:

	void onPlayerConnect(nsocket_t socket);

	static int adminClientThreadMain(void* param);



private:

		/// Local admin client.
	MCClient* m_adminClient;

		/// Thread running the local client.
	thread m_adminClientThread;

	bool m_headless;
};
