#include "mc/MC.h"
#include "WorldGenerator.h"

#include "mc/world/World.h"



static int min(int a, int b) { return a < b ? a : b; }



WorldGenerator::WorldGenerator(World* w, bool circular)
	: m_world(w), m_circular(circular)
{

}


void WorldGenerator::generate()
{
	this->generateMines();
	this->spawnItems();

	this->generateTreasure();
}



void WorldGenerator::generateMines()
{
	World::Tile* t = m_world->m_tiles;

	if (!m_circular) {
		for (int i = 0; i < m_world->getSizeY(); i++) {
			int d = min(i, m_world->getSizeY() - i - 1);

			if (d < 3) {
				for (int j = 0; j < m_world->getSizeX(); ++j) {
					t->m_material = 1;
					t->m_broken = true;

					t++;
				}
			}
			else if (d < 8) {
				for (int j = 0; j < m_world->getSizeX(); ++j) {
					t->m_material = 1;
					t->m_broken = (rand() / (float) RAND_MAX) < ((8.0F - d - 1) / 5.0F);

					t++;
				}
			}
			else if (d < 12) {
				for (int j = 0; j < m_world->getSizeX(); ++j) {
					t->m_material = 1;

					t++;
				}
			}
			else {
				for (int j = 0; j < m_world->getSizeX(); ++j) {
					t->m_material = ((rand() % 100) < 3) ? 4 : 2;;

					t++;
				}
			}
		}
	}
	else {
		for (int i = 0; i < m_world->getSizeY(); i++) {
			for (int j = 0; j < m_world->getSizeX(); ++j) {
				int d = min(min(i, m_world->getSizeY() - i - 1), min(j, m_world->getSizeX() - j - 1));

				if (d < 3) {
					t->m_material = 1;
					t->m_broken = true;
				}
				else if (d < 8) {
					t->m_material = 1;
					t->m_broken = (rand() / (float) RAND_MAX) < ((8.0F - d - 1) / 5.0F);
				}
				else if (d < 12) {
					t->m_material = 1;
				}
				else {
					t->m_material = ((rand() % 100) < 3) ? 4 : 2;
				}

				t++;
			}
		}
	}
}

void WorldGenerator::generateTreasure()
{
	// Generate treasure structure.
	int centerX = m_world->getSizeX() / 2;
	int centerY = m_world->getSizeY() / 2;
	m_world->m_tiles[centerY * m_world->getSizeX() + centerX].m_broken = true;

	for (int y = -3; y <= 3; ++y) {
		for (int x = -3; x <= 3; ++x) {
			if (x * x + y * y > 10.0F) continue;
			m_world->m_tiles[(y + centerY) * m_world->getSizeX() + (x + centerX)].m_material = 3;
		}
	}

	m_world->setItemAt(1, centerX, centerY);
}

void WorldGenerator::spawnItems()
{
	// Random objects test.
	for (int i = 0; i < 120; ++i) {
		int xPos = m_circular ? (rand() % (m_world->getSizeX() - 6) + 3) : (rand() % m_world->getSizeX());
		int yPos = rand() % (m_world->getSizeY() - 6) + 3;

		if (m_world->isTileBroken(xPos, yPos)) continue;

		m_world->setItemAt(1, xPos, yPos);
	}
}
