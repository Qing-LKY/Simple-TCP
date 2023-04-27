#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int server_prework();
void server_listener();
void server_connection();

#endif