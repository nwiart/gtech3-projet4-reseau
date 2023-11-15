#pragma once

#include <stdlib.h>
#include <stdint.h>



class MC
{
public:

	static void init();

	static const uint16_t SERVER_PORT = 27015;
	static const uint16_t WEB_PORT    = SERVER_PORT + 1;
};
