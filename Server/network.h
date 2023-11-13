#pragma once

#include <stdint.h>


typedef uint64_t nsocket_t;



// Platform-specific initialization.
bool network_init();

void network_quit();


// Server-side.
nsocket_t network_setup_server(uint16_t port);

void network_shutdown_server(nsocket_t);
