#ifndef ARGS_H
#define ARGS_H

#include <arpa/inet.h>

extern int is_server;
extern int queue;
extern struct sockaddr_in socket_addr;
extern char upload_file[];

int parse_args(int argc, char *argv[]);

#endif