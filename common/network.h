#pragma once

#include <stdint.h>

#define N_MAKE_IPV4(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d))


typedef int64_t nsocket_t;

typedef void (*PacketHandler)(nsocket_t, const struct PacketBase& b, void* param);
typedef void (*AcceptHandler)(nsocket_t, void* param);
typedef void (*CloseHandler)(nsocket_t, void* param);



// Platform-specific initialization.
bool network_init();

void network_quit();


// Server-side.
nsocket_t network_setup_server(uint16_t port, AcceptHandler acceptHandler, CloseHandler closeHandler, PacketHandler packetHandler, void* handlerParam);

nsocket_t network_setup_web_server(uint16_t port);

// Used on Win32 to pickup events from an invisible window.
void network_server_poll_events();

void network_shutdown_server(nsocket_t socket);


// Client-side.
nsocket_t network_setup_client4(uint32_t addr, uint16_t port, PacketHandler h, void* handlerParam);

void network_shutdown_client(nsocket_t socket);

// Used on Win32 to pickup events from an invisible window.
// DO NOT use network_server_poll_events(), this function executes actions intended for server only.
void network_client_poll_events();
