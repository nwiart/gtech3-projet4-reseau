#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

using namespace std;



void threadTest();



int main(int argc, char** argv)
{
	cout << "Enter your name : ";

	string name;
	getline(cin, name);


	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	addrinfo hints, *result;
	ZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(0, "27015", &hints, &result);
	SOCKET connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
	cout << "Trying to reach server...\n";
	while (connect(connectSocket, result->ai_addr, result->ai_addrlen) != 0);

	freeaddrinfo(result);

	sf::RenderWindow window;
	window.create(sf::VideoMode(600, 600), "TIC TAC TOE", sf::Style::Close);
	window.setVerticalSyncEnabled(true);



	sf::RectangleShape lines[4];
	lines[0].setSize(sf::Vector2f(600, 2)); lines[0].setPosition(sf::Vector2f(0, 200));
	lines[1].setSize(sf::Vector2f(600, 2)); lines[1].setPosition(sf::Vector2f(0, 400));
	lines[2].setSize(sf::Vector2f(2, 600)); lines[2].setPosition(sf::Vector2f(200, 0));
	lines[3].setSize(sf::Vector2f(2, 600)); lines[3].setPosition(sf::Vector2f(400, 0));


	threadTest();


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear();

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		for (int y = 0; y < 600; y += 200) {
			for (int x = 0; x < 600; x += 200) {
				
				if (mousePos.x >= x && mousePos.x < x + 200 && mousePos.y >= y && mousePos.y < y + 200) {
					sf::RectangleShape over(sf::Vector2f(200, 200));
					over.setPosition(sf::Vector2f(x, y));
					over.setFillColor(sf::Color(60, 60, 60, 255));
					window.draw(over);
				}
			}
		}

		for (const sf::RectangleShape* ps = lines; ps != lines + 4; ps++) {
			window.draw(*ps);
		}
		window.display();
	}



	closesocket(connectSocket);

	WSACleanup();

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
