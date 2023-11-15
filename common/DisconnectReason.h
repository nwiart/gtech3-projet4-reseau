#pragma once

#include "enum.h"



struct DisconnectReason : kdEnum<uint32_t>
{
	enum
	{
		UNKNOWN = 0,
		SERVER_CLOSED,
	};

	DisconnectReason() { }
	DisconnectReason(uint32_t v) { m_value = v; }
	void operator=(uint32_t v) { m_value = v; }

	const char* toString() const;
};
