#pragma once



void network_initialize();
void network_shutdown();


void network_connect(const char* port);

void network_setup_server(const char* port);
