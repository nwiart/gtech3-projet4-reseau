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
	cout << "Enter your name : ";

	string name;
	getline(cin, name);


	game_initialize();

	network_initialize();
	network_connect(0);


	threadTest();


	while (game_is_running())
	{
		game_step();
	}



	network_shutdown();

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
