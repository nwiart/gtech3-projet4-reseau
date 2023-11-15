#pragma once

#include "network.h"
#include "thread.h"

class MCClient;



class MCServer
{
public:

		/// Server application's main entry point.
	static int main(bool headless);

	
	MCServer(bool headless);
	~MCServer();

	void host();



private:

		/// Local admin client.
	MCClient* m_adminClient;
};
