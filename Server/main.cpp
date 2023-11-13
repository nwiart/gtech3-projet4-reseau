#include "network.h"
#include "thread.h"

#include "web.h"

#include <iostream>
#include <string>

using namespace std;

#include <Windows.h>



static int acceptThread(void* p)
{
	nsocket_t& listenSocket = *((nsocket_t*)p);

	while (true)
	{
		//nsocket_t clientSocket = accept(listenSocket, 0, 0);

		//cout << "WE GOT ONE!!!\n";

		Sleep(1);
	}

	return 0;
}



int main(int argc, char** argv)
{
	if (!network_init()) {
		std::cout << "Network initialization failed!\n";
		return 1;
	}

	uint16_t gamePort = 27015;
	uint16_t webPort  = 27016;

	nsocket_t listenSocket = network_setup_server(gamePort);
	
	cout << "TIC TAC TOE Server\n";
	cout << "---\n\n";

	thread t(acceptThread, &listenSocket);
	t.start();
	cout << "Now listening on " << gamePort << "...\n";


	web_start_server(webPort);
	cout << "Web server broadcasting on " << webPort << "...\n";


	while (1) {
		Sleep(1);
	}

	/*string s;
	while (true) {
		cout << "> ";
		getline(cin, s);

		if (s == "exit") {
			break;
		}
	}*/



	network_quit();

	return 0;
}
