#include "mc/MC.h"



MC* MC::m_instance = 0;



MC::MC()
	: m_world(0)
	, m_localPlayer(0)
{
	m_instance = this;
}



void MC::init()
{

}
