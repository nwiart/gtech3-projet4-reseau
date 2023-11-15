#pragma once

class World;



class Player
{
	friend class World;

public:

	Player();

	void move(int dx, int dy);

	bool isRemote() const;


private:

	World* m_world;

	int m_xPos;
	int m_yPos;
};
