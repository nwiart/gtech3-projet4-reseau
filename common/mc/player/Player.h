#pragma once

#include <string>

class World;
class Inventory;



class Player
{
	friend class World;

public:

	Player();
	~Player();

	void move(int dx, int dy);
	void teleport(int x, int y);

	bool isRemote() const;


	inline World* getWorld() const { return m_world; }

	inline Inventory* getInventory() const { return m_inventory; }

	inline const std::string& getName() const { return m_name; }
	inline int getID() const { return m_id; }

	inline int getPosX() const { return m_xPos; }
	inline int getPosY() const { return m_yPos; }

	inline void setName(const char* name) { m_name = name; }


private:

	World* m_world;

	Inventory* m_inventory;

	std::string m_name;
	int m_id;

	int m_xPos;
	int m_yPos;
};
