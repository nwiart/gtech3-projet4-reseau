#include "mc/MC.h"
#include "MCClient.h"

#include "mc/world/World.h"
#include "mc/player/Player.h"

#include "mc/registry/TileRegistry.h"

#include "mc/network/client/MCClientPacketHandler.h"

#include "mc/gui/GuiStatus.h"
#include "mc/gui/GuiMainMenu.h"
#include "mc/gui/GuiInventory.h"

#include <iostream>
#include <string>
using namespace std;



MCClient* MCClient::m_instance = 0;

string localPlayerName = "TheVid";



int MCClient::main()
{
	if (!network_init()) {
		std::cout << "Network initialization failed!\n";
		return 1;
	}

	srand(time(0));

	MC mc;
	mc.init();


	MCClient* client = new MCClient("MC Client");

	GuiMainMenu* menu = new GuiMainMenu();
	client->openGui(menu);

	// Main loop.
	client->run();

	delete client;

	network_quit();

	return 0;
}



MCClient::MCClient(const char* windowTitle)
	: m_connectThread(&MCClient::connectThreadMain, this)
	, m_gui(0)
	, m_nextGui(0)
	, m_guiSwitch(false)
	, m_showStatus(false)
{
	m_instance = this;

	m_windowTitle = windowTitle;
	m_frame = 0;

	m_font.loadFromFile("C:/Windows/Fonts/consola.ttf");
	m_font.setSmooth(false);
}

MCClient::~MCClient()
{

}



void MCClient::run()
{
	// Create window.
	m_window.create(sf::VideoMode(640, 640), m_windowTitle.c_str(), sf::Style::Close);
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);



	// Main loop.
	while (m_window.isOpen())
	{
		if (m_showStatus) {
			m_showStatus = false;
			this->displayStatus();
		}

		// Gui open mechanism.
		if (m_guiSwitch) {
			if (m_gui) delete m_gui;
			m_gui = m_nextGui;
			m_guiSwitch = false;
		}

		// Window events.
		sf::Event event;
		while (m_window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::KeyPressed:
				if (m_gui) break;
				switch (event.key.code)
				{
				case sf::Keyboard::Z: case sf::Keyboard::Up:    MC::getInstance().getLocalPlayer()->move( 0,  1); break;
				case sf::Keyboard::S: case sf::Keyboard::Down:  MC::getInstance().getLocalPlayer()->move( 0, -1); break;
				case sf::Keyboard::D: case sf::Keyboard::Right: MC::getInstance().getLocalPlayer()->move( 1,  0); break;
				case sf::Keyboard::Q: case sf::Keyboard::Left:  MC::getInstance().getLocalPlayer()->move(-1,  0); break;

				case sf::Keyboard::Tab: break;
				}
				break;

			case sf::Event::TextEntered:
				if (m_gui) {
					m_gui->onTextInput(event.text.unicode);
				}
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (m_gui) {
						m_gui->onLeftClick(event.mouseButton.x, event.mouseButton.y);
					}
				}
				break;
			}
		}

		// Render.
		this->render();

		m_frame++;
	}

	// TODO : clean exit.
	m_connectThread.stop();

	network_shutdown_client(m_serverSocket);
}

void MCClient::render()
{
	m_window.clear();

	Player* player = MC::getInstance().getLocalPlayer();

	if (player)
	{
		sf::View view;
		view.setSize(sf::Vector2f(20.0F, -20.0F));
		view.setCenter(player ? sf::Vector2f(player->getPosX() + 0.5F, player->getPosY() + 0.5F) : sf::Vector2f(7.5F, 15.5F));
		m_window.setView(view);

		World* world = MC::getInstance().getLocalWorld();

		for (int y = max(0, player->getPosY() - 10); y < min(world->getSizeY(), player->getPosY() + 11); y++) {

			for (int x = max(0, player->getPosX() - 10); x < min(world->getSizeX(), player->getPosX() + 11); x++) {

				const World::Tile* tiles = world->getTiles() + y * world->getSizeX() + x;

				uint16_t tileID = tiles->m_material;
				uint16_t itemID = tiles->m_item;

				if (tileID) {
					sf::Color c = tiles->m_broken ? TileRegistry::getTile(tileID).getBrokenColor() : TileRegistry::getTile(tileID).getColor();

					sf::RectangleShape r;
					r.setSize(sf::Vector2f(1, 1));
					r.setFillColor(c);
					r.setPosition(sf::Vector2f(x, y));
					m_window.draw(r);
				}

				if (itemID && tiles->m_broken) {
					sf::RectangleShape r;
					r.setSize(sf::Vector2f(0.6F, 0.25F));
					r.setOrigin(r.getSize() * 0.5F);
					r.setFillColor(sf::Color(200, 200, 200));
					r.setPosition(sf::Vector2f(x + 0.5F, y + 0.5F + sin(m_frame * 0.0166F * 3.2F) * 0.2F));
					m_window.draw(r);
				}

				tiles++;
			}
		}
	
		for (const std::pair<int, Player*>& pl : world->getPlayers()) {
			Player* p = pl.second;

			// Player rect.
			sf::RectangleShape r;
			r.setSize(sf::Vector2f(0.6F, 0.6F));
			r.setOrigin(r.getSize() * 0.5F);
			r.setFillColor(sf::Color::White);
			r.setPosition(sf::Vector2f(p->getPosX() + 0.5F, p->getPosY() + 0.5F));
			m_window.draw(r);

			// Player name.
			sf::Text t(p->getName(), MCClient::getInstance().getGlobalFont(), 12);
			t.setScale(sf::Vector2f(1.0F, -1.0F) / 16.0F);
			t.setOrigin(t.getLocalBounds().getSize() * 0.5F);
			t.setPosition(sf::Vector2f(p->getPosX() + 0.5F, p->getPosY() + 2.0F));
			m_window.draw(t);
		}
	}

	sf::View view;
	view.setCenter(sf::Vector2f(10.0F, 10.0F) * 16.0F);
	view.setSize(sf::Vector2f(20.0F, 20.0F) * 16.0F);
	m_window.setView(view);

	if (m_inventoryUI) {
		m_inventoryUI->render(m_window);
	}

	if (m_gui) {
		m_gui->render(m_window);
	}

	m_window.display();
}


void MCClient::connect(const char* stringip, uint16_t port)
{
	int ip[4];
	if (stringip && stringip[0]) {
		sscanf_s(stringip, "%d.%d.%d.%d", ip + 0, ip + 1, ip + 2, ip + 3);
	}

	m_serverIP4 = N_MAKE_IPV4(ip[0], ip[1], ip[2], ip[3]);

	// Connect to server.
	cout << "\nTrying to reach server at " << stringip << ':' << MC::SERVER_PORT << "...\n";

	m_connectThread.start();
}

void MCClient::sendPacket(const PacketBase& b)
{
	packet_send(m_serverSocket, b);
}

void MCClient::displayStatus()
{
	GuiStatus* status = new GuiStatus();
	this->openGui(status);
}



int MCClient::connectThreadMain(void* param)
{
	MCClient* client = reinterpret_cast<MCClient*>(param);

	GuiStatus::setStatus("Connecting to server...", 0, false);
	GuiStatus::displayStatus();

	client->m_serverSocket = network_setup_client4(client->m_serverIP4, MC::SERVER_PORT, &MCClientPacketHandler::handleClose, &MCClientPacketHandler::handlePacket, client);

	if (client->m_serverSocket == -1) {
		GuiStatus::setStatus("Failed to reach server", "Unknown error", true);
		GuiStatus::displayStatus();
	}
	else {
		// Send join information.
		Packet<ClientJoinPacket> packet;
		memcpy(packet->m_name, localPlayerName.c_str(), localPlayerName.size() + 1);
		packet->m_isSpectator = true;
		client->sendPacket(packet);
	
		network_client_poll_events();
	}

	return 0;
}

void MCClient::onServerClose()
{
	
}
