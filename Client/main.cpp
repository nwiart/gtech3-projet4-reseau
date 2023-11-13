#include <iostream>
#include <string>

#include "game.h"
#include "network.h"

#include "thread.h"

using namespace std;



int clientThread(void* p)
{
	network_setup_client4(*((uint32_t*)p), 27015);

	return 0;
}



int main(int argc, char** argv)
{
	// Ask for IP and name.
	cout << "Enter server IP : ";
	string sip;
	getline(cin, sip);

	int ip[4];
	if (!sip.empty()) {
		sscanf_s(sip.c_str(), "%d.%d.%d.%d", ip+0, ip+1, ip+2, ip+3);
	}

	cout << "Enter your name : ";

	string name;
	getline(cin, name);


	// Init.
	game_initialize();

	network_init();


	cout << "\nTrying to reach server at " << sip << ':' << 27015 << "...\n";

	uint32_t packedIPV4 = N_MAKE_IPV4(ip[0], ip[1], ip[2], ip[3]);
	thread t(&clientThread, &packedIPV4);
	t.start();


	while (game_is_running())
	{
		game_step();
	}



	network_quit();

	return 0;
}
