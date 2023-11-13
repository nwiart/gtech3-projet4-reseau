#pragma once

#include <stdint.h>

#define N_MAKE_IPV4(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d))


typedef int64_t nsocket_t;



// Platform-specific initialization.
bool network_init();

void network_quit();


// Server-side.
nsocket_t network_setup_server(uint16_t port);

void network_shutdown_server(nsocket_t socket);


// Client-side.
nsocket_t network_setup_client4(uint32_t addr, uint16_t port);