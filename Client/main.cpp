#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <iostream>
#include <string>

#include "game.h"
#include "network.h"

using namespace std;



void threadTest();



int main(int argc, char** argv)
{
	cout << "Enter server IP : ";
	int ip[4];
	scanf_s("%d.%d.%d.%d", ip+0, ip+1, ip+2, ip+3);

	int c;
	while ( (c = getchar()) && c != EOF && c != '\n');

	cout << "Enter your name : ";

	string name;
	getline(cin, name);


	game_initialize();

	network_init();
	
	//network_setup_client4(N_MAKE_IPV4(192, 128, 43, 250), 27015);
	network_setup_client4(N_MAKE_IPV4(ip[0], ip[1], ip[2], ip[3]), 27015);


	threadTest();


	while (game_is_running())
	{
		game_step();
	}



	network_quit();

	return 0;
}



DWORD a(void* p)
{
	return 1;
}

void threadTest()
{
	DWORD threadID;
	HANDLE threadHandle = CreateThread(0, 0, a, 0, 0, &threadID);

	cout << "Thread created with ID " << threadID << '\n';

	WaitForSingleObject(threadHandle, INFINITE);

	DWORD exitCode;
	GetExitCodeThread(threadHandle, &exitCode);

	cout << "Thread exited with code " << exitCode << '\n';

	CloseHandle(threadHandle);
}
