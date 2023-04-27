#include "args.h"
#include "server.h"
#include "client.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int err = 0;
    err = parse_args(argc, argv);
    if (err != 0) {
        puts("Error when parse command!");
        return 1;
    }
    if (is_server) {
        if (server_prework() == -1) return -1;
        server_listener();
    } else {
        if (client_prework() == -1) return -1;
        if (client_upload() == -1) return -1;
        if (client_recv() == -1) return -1;
    }
    return 0;
}