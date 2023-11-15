#include "mc/MC.h"
#include "MCClient.h"

#include "mc/network/client/MCClientPacketHandler.h"

#include <iostream>
#include <string>
using namespace std;



int MCClient::main()
{
	network_init();

	// Ask for IP and name.
	cout << "Enter server IP : ";
	string sip;
	getline(cin, sip);

	
	//cout << "Enter your name : ";
	//getline(cin, name);


	MCClient* client = new MCClient();

	client->connect(sip.c_str(), MC::SERVER_PORT);

	while (client->m_window.isOpen())
	{
		sf::Event event;
		while (client->m_window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				client->m_window.close();
				break;

			case sf::Event::MouseButtonReleased:
				break;
			}
		}
	}

	delete client;

	network_quit();

	return 0;
}



MCClient::MCClient()
	: m_connectThread(&MCClient::connectThreadMain, this)
{
	m_window.create(sf::VideoMode(640, 640), "MC");
}

void MCClient::connect(const char* stringip, uint16_t port)
{
	int ip[4];
	if (stringip && stringip[0]) {
		sscanf_s(stringip, "%d.%d.%d.%d", ip + 0, ip + 1, ip + 2, ip + 3);
	}

	m_serverIP4 = N_MAKE_IPV4(ip[0], ip[1], ip[2], ip[3]);

	cout << "\nTrying to reach server at " << stringip << ':' << MC::SERVER_PORT << "...\n";
	m_connectThread.start();
}


int MCClient::connectThreadMain(void* param)
{
	MCClient* client = reinterpret_cast<MCClient*>(param);

	client->m_serverSocket = network_setup_client4(client->m_serverIP4, MC::SERVER_PORT, &MCClientPacketHandler::response);

	cout << "Connection success!\n";

	/*Packet<SetNamePacket> packet;
	memcpy(packet->name, name.c_str(), name.size() + 1);
	packet_send(serverSocket, packet);

	char buf[PACKET_MAX_SIZE];
	packet_recv(serverSocket, buf);

	response(serverSocket, *((const PacketBase*)buf));*/

	return 0;
}
