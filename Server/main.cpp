#include "network.h"

#include <iostream>
#include <string>

using namespace std;


//void threadTest(SOCKET* listener);



int main(int argc, char** argv)
{
	if (!network_init()) {
		std::cout << "Network initialization failed!\n";
		return 1;
	}

	uint16_t port = 27015;
	nsocket_t listenSocket = network_setup_server(port);
	
	cout << "TIC TAC TOE Server\n";
	cout << "---\n\n";
	cout << "Now listening on " << port << "...\n";


	/*string s;
	while (true) {
		cout << "> ";
		getline(cin, s);

		if (s == "exit") {
			break;
		}
	}*/


	//int clientSocket = accept(listenSocket, (sockaddr*) &addr, &addrLen);

	//std::cout << "INCOMING\n";

	network_quit();
	return 0;
}


/*DWORD a(void* p)
{
	SOCKET& listenSocket = *((SOCKET*) p);

	SOCKET clientSocket = accept(listenSocket, 0, 0);

	cout << "WE GOT ONE!!!\n";

	return 0;
}

void threadTest(SOCKET* listener)
{
	DWORD threadID;
	HANDLE threadHandle = CreateThread(0, 0, a, listener, 0, &threadID);

	cout << "Thread created with ID " << threadID << '\n';

	//WaitForSingleObject(threadHandle, INFINITE);

	//DWORD exitCode;
	//GetExitCodeThread(threadHandle, &exitCode);

	//std::cout << "Thread exited with code " << exitCode << '\n';

	//CloseHandle(threadHandle);
}*/
