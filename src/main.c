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
        printf("Prework finished!\n");
        printf("Server address: %s Port: %d\n", inet_ntoa(socket_addr.sin_addr), ntohs(socket_addr.sin_port));
        server_listener();
    } else {
        if (client_prework() == -1) return -1;
        if (client_recv() == -1) return -1;
        if (client_upload() == -1) return -1;
        sleep(3); // wait for recv pthread
    }
    return 0;
}