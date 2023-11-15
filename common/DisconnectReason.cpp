#include "mc/MC.h"
#include "DisconnectReason.h"



const char* DisconnectReason::toString() const
{
	switch (this->operator uint32_t())
	{
	case SERVER_CLOSED: return "Server closed";
	default: return "Connection lost (unknown reason)";
	}
}
