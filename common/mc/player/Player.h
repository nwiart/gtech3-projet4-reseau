#pragma once

#include <string>

class World;



class Player
{
	friend class World;

public:

	Player();

	void move(int dx, int dy);
	void teleport(int x, int y);

	bool isRemote() const;


	inline World* getWorld() const { return m_world; }

	inline const std::string& getName() const { return m_name; }

	inline int getPosX() const { return m_xPos; }
	inline int getPosY() const { return m_yPos; }

	inline void setName(const char* name) { m_name = name; }


private:

	World* m_world;

	std::string m_name;

	int m_xPos;
	int m_yPos;
};
