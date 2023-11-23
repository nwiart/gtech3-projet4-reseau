#pragma once

#include "enum.h"



struct ConnectionDenialReason : public kdEnum<uint32_t>
{
	enum
	{
		ALLOWED,
		BAD_NAME,
		NAME_TAKEN,
		SERVER_FULL,
	};

	ConnectionDenialReason() { }
	ConnectionDenialReason(uint32_t v) { m_value = v; }
	void operator=(uint32_t v) { m_value = v; }

	const char* toString() const;
};
