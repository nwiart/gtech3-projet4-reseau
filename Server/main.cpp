#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <iostream>
#include <string>

using namespace std;



void threadTest(SOCKET* listener);



int main(int argc, char** argv)
{
	const char PORT[] = "27015";

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	addrinfo hints, *result;

	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(0, PORT, &hints, &result);
	SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(listenSocket, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);

	listen(listenSocket, SOMAXCONN);

	cout << "TIC TAC TOE Server\n";
	cout << "\n---\n\n";
	cout << "Now listening on " << PORT << "...\n";

	
	threadTest(&listenSocket);

	string s;
	while (true) {
		cout << "> ";
		getline(cin, s);

		if (s == "exit") {
			break;
		}
	}


	closesocket(listenSocket);

	WSACleanup();

	return 0;
}



DWORD a(void* p)
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
}
