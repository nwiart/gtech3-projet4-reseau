#pragma once

#include <SFML/Graphics.hpp>

#include "network.h"
#include "thread.h"

#include <string>

class Gui;
class GuiInventory;



class MCClient
{
	friend class MCClientPacketHandler;

public:

		/// Client application's main entry point.
	static int main();

	static inline MCClient& getInstance() { return *m_instance; }


	MCClient(const char* windowTitle);
	~MCClient();

	void run();

		/// Connect to a remote server.
	void connect(const char* ip, uint16_t port);

	void sendPacket(const PacketBase& b);

	inline sf::RenderWindow& getWindow() { return m_window; }
	inline const sf::Font& getGlobalFont() const { return m_font; }

	inline void openGui(Gui* gui) { m_nextGui = gui; m_guiSwitch = true; }

private:

	static int connectThreadMain(void* param);

	void onServerClose();

	void render();


private:

	static MCClient* m_instance;

	sf::RenderWindow m_window;
	sf::Font m_font;

	Gui* m_gui;
	Gui* m_nextGui;
	bool m_guiSwitch;

	GuiInventory* m_inventoryUI;

	thread m_connectThread;
	nsocket_t m_serverSocket;
	uint32_t m_serverIP4;

	std::string m_windowTitle;

	int m_frame;
};
