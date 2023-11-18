#pragma once

class World;



class WorldGenerator
{
public:

	WorldGenerator(World* w, bool circular);

	void generate();


private:

	void generateMines();

	void generateTreasure();

	void spawnItems();


private:

	World* m_world;
	bool m_circular;
};
