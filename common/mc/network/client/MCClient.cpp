#include "mc/MC.h"
#include "MCClient.h"

#include "mc/network/client/MCClientPacketHandler.h"

#include <iostream>
#include <string>
using namespace std;



MCClient* MCClient::m_instance = 0;

string name;

int MCClient::main()
{
	// Ask for IP and name.
	string stringip;

	cout << "Enter your name : "; getline(cin, name);
	cout << "Enter server IP : "; getline(cin, stringip);


	network_init();


	MCClient* client = new MCClient("MC Client");

	// Connect to server.
	cout << "\nTrying to reach server at " << stringip << ':' << MC::SERVER_PORT << "...\n";
	client->connect(stringip.c_str(), MC::SERVER_PORT);

	// Main loop.
	client->run();

	delete client;

	network_quit();

	return 0;
}



MCClient::MCClient(const char* windowTitle)
	: m_connectThread(&MCClient::connectThreadMain, this)
{
	m_instance = this;

	m_windowTitle = windowTitle;
}

void MCClient::run()
{
	// Create window.
	m_window.create(sf::VideoMode(640, 640), m_windowTitle.c_str(), sf::Style::Close);
	m_window.setVerticalSyncEnabled(true);

	// Main loop.
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::MouseButtonReleased:
				break;
			}
		}
	}
}

void MCClient::connect(const char* stringip, uint16_t port)
{
	int ip[4];
	if (stringip && stringip[0]) {
		sscanf_s(stringip, "%d.%d.%d.%d", ip + 0, ip + 1, ip + 2, ip + 3);
	}

	m_serverIP4 = N_MAKE_IPV4(ip[0], ip[1], ip[2], ip[3]);

	m_connectThread.start();
}

void MCClient::sendPacket(const PacketBase& b)
{
	packet_send(m_serverSocket, b);
}


#include <Windows.h>
int MCClient::connectThreadMain(void* param)
{
	MCClient* client = reinterpret_cast<MCClient*>(param);

	client->m_serverSocket = network_setup_client4(client->m_serverIP4, MC::SERVER_PORT, &MCClientPacketHandler::handlePacket, client);

	Packet<SetNamePacket> packet;
	memcpy(packet->name, name.c_str(), name.size() + 1);
	client->sendPacket(packet);


	while (1) {
		network_client_poll_events();

		Sleep(1);
	}

	return 0;
}
