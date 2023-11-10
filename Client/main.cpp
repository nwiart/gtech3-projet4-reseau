#include <Windows.h>

#include <SFML/Graphics.hpp>

#include <iostream>



void threadTest();



#ifdef _DEBUG
int main(int argc, char** argv)
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(600, 600), "TIC TAC TOE", sf::Style::Close);



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
		for (const sf::RectangleShape* ps = lines; ps != lines + 4; ps++) {
			window.draw(*ps);
		}
		window.display();
	}

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

	std::cout << "Thread created with ID " << threadID << '\n';

	WaitForSingleObject(threadHandle, INFINITE);

	DWORD exitCode;
	GetExitCodeThread(threadHandle, &exitCode);

	std::cout << "Thread exited with code " << exitCode << '\n';

	CloseHandle(threadHandle);
}
