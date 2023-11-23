#include "mc/MC.h"
#include "ConnectionDenialReason.h"



const char* ConnectionDenialReason::toString() const
{
	switch (this->operator uint32_t())
	{
	case ALLOWED:     return "Joined successfully!";
	case BAD_NAME:    return "The name contains invalid characters.";
	case NAME_TAKEN:  return "This name is already taken.";
	case SERVER_FULL: return "There are already two players currently in game.";
	default: return "Unknown error.";
	}
}
