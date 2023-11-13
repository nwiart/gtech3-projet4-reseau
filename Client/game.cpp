#include "game.h"

#include <SFML/Graphics.hpp>

#include <string.h>



#define GRID_DIMENSIONS 3
#define GRID_SQUARE_SIZE 200

#define WINDOW_SIZE (GRID_SQUARE_SIZE * GRID_DIMENSIONS)
static const char WINDOW_TITLE[] = "TIC TAC TOE";

int grid[GRID_DIMENSIONS][GRID_DIMENSIONS];

static int current_player;
static int winner;
static struct
{
	int row : 2;
	int col : 2;
	int diagonal : 3;
}
win_config;



struct render_t
{
	sf::RenderWindow window;

	sf::RectangleShape gridLines[4];
};

static render_t& g_render() { static render_t r; return r; }

static void game_render();



void game_initialize()
{
	g_render().window.create(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), WINDOW_TITLE, sf::Style::Close);
	g_render().window.setVerticalSyncEnabled(true);

	g_render().gridLines[0].setSize(sf::Vector2f(WINDOW_SIZE, 2)); g_render().gridLines[0].setPosition(sf::Vector2f(0, GRID_SQUARE_SIZE * 1));
	g_render().gridLines[1].setSize(sf::Vector2f(WINDOW_SIZE, 2)); g_render().gridLines[1].setPosition(sf::Vector2f(0, GRID_SQUARE_SIZE * 2));
	g_render().gridLines[2].setSize(sf::Vector2f(2, WINDOW_SIZE)); g_render().gridLines[2].setPosition(sf::Vector2f(GRID_SQUARE_SIZE * 1, 0));
	g_render().gridLines[3].setSize(sf::Vector2f(2, WINDOW_SIZE)); g_render().gridLines[3].setPosition(sf::Vector2f(GRID_SQUARE_SIZE * 2, 0));


	gameplay_reset();
}

void game_step()
{
	sf::Event event;
	while (g_render().window.pollEvent(event)) {
		switch (event.type)
		{
		case sf::Event::Closed:
			g_render().window.close();
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == 0) {
				gameplay_play(event.mouseButton.x / 200, event.mouseButton.y / 200);
			}
			break;
		}
	}

	game_render();
}

bool game_is_running()
{
	return g_render().window.isOpen();
}

static void game_render()
{
	sf::RenderWindow& window = g_render().window;

	window.clear();

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	// Tile hovering.
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

	// Placed tiles.
	for (int y = 0; y < GRID_DIMENSIONS; ++y) {
		for (int x = 0; x < GRID_DIMENSIONS; ++x) {
			if (grid[y][x] == 0) continue;

			sf::RectangleShape r(sf::Vector2f(140, 140));
			r.setPosition(sf::Vector2f(x * 200 + 30, y * 200 + 30));
			r.setFillColor(grid[y][x] == 1 ? sf::Color::Blue : sf::Color::Red);
			window.draw(r);
		}
	}

	// Game grid.
	for (const sf::RectangleShape* ps = g_render().gridLines; ps != g_render().gridLines + 4; ps++) {
		window.draw(*ps);
	}

	// Win.
	if (winner != 0) {
		//sf::RectangleShape win;
		//window.draw(win);
	}

	window.display();
}



void gameplay_reset()
{
	memset(grid, 0, 9 * sizeof(int));

	current_player = 1;
	winner = 0;
}

static bool horizontal_win(int y);
static bool vertical_win(int x);
static bool diagonal_win();

#include <iostream>
void gameplay_play(int x, int y)
{
	if (grid[y][x] != 0) return;

	grid[y][x] = current_player;

	horizontal_win(y) || vertical_win(x) || diagonal_win();

	current_player = current_player ^ 0b11;
}


static bool horizontal_win(int y)
{
	for (int dx = 0; dx < GRID_DIMENSIONS; dx++) {
		if (grid[y][dx] != current_player) {
			return false;
		}
	}

	winner = current_player;
	win_config.col = 0;
	win_config.row = y;
	win_config.diagonal = 0;
	return true;
}

static bool vertical_win(int x)
{
	for (int dy = 0; dy < GRID_DIMENSIONS; dy++) {
		if (grid[dy][x] != current_player) {
			return false;
		}
	}

	winner = current_player;
	win_config.col = x;
	win_config.row = 0;
	win_config.diagonal = 0;
	return true;
}

static bool diagonal_win()
{
	bool wl = true, wr = true;

	for (int i = 0; i < GRID_DIMENSIONS; i++) {
		if (grid[i][i] != current_player) {
			wl = false;
			break;
		}
	}

	for (int i = 0; i < GRID_DIMENSIONS; i++) {
		if (grid[GRID_DIMENSIONS - i - 1][i] != current_player) {
			wr = false;
			break;
		}
	}

	if (wl) {
		winner = current_player;
		win_config.col = 0;
		win_config.row = 0;
		win_config.diagonal = 1;
	}
	else if (wr) {
		winner = current_player;
		win_config.col = 0;
		win_config.row = 0;
		win_config.diagonal = 2;
	}

	return wl || wr;
}
