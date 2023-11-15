#pragma once

#include <SFML/Graphics.hpp>

#include "network.h"
#include "thread.h"



class MCClient
{
public:

		/// Client application's main entry point.
	static int main();


	MCClient();

	void connect(const char* ip, uint16_t port);

	inline sf::RenderWindow& getWindow() { return m_window; }

private:

	static int connectThreadMain(void* param);


private:

	sf::RenderWindow m_window;

	thread m_connectThread;
	nsocket_t m_serverSocket;
	uint32_t m_serverIP4;
};
