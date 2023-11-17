#include "mc/MC.h"
#include "DisconnectReason.h"



const char* DisconnectReason::toString() const
{
	switch (this->operator uint32_t())
	{
	case SERVER_CLOSED: return "Server closed";
	case PLAYER_LEFT: return "Player left";
	default: return "Connection lost (unknown reason)";
	}
}
