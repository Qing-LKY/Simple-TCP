#include "args.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

in_port_t port = 5555;
int is_server = 0;
int queue = 5;
int set_addr = 0;
struct sockaddr_in socket_addr;
char upload_file[100];

struct option opts[] = {
    {"port", required_argument, 0, 'p'},
    {"addr", required_argument, 0, 'a'},
    {"queue", required_argument, 0, 'q'},
    {"server", no_argument, 0, 's'},
    {"file", required_argument, 0, 'f'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

char optstr[] = "p:a:q:f:sh";

int copy_string(char *cnf, const char *arg) {
    int len = strlen(arg);
    // too long file/directoty name
    if (len >= 50) return 1;
    // copy string
    memcpy(cnf, arg, len + 1);
    return 0;
}

int copy_number(int *cnf, const char *arg) {
    char *endptr;
    errno = 0;
    // set 0 to auto choose 10, 8 or 16
    *cnf = strtol(arg, &endptr, 0);
    // something wrong when convert
    if (errno != 0 || *endptr != 0) return 2;
    return 0;
}

void display_help(char *arg0) {
    printf("Usage: %s [-s [-q 5]] [-a 127.0.0.1] [-p 8888]\n", arg0);
    printf(" -s, --server      start as a server\n");
    printf(" -q, --queue NUM   set max number of pending connections (ignore if client)\n");
    printf(" -a, --addr IPv4   set address (numbers-and-dots notation)\n");
    printf(" -p, --port NUM    set port\n");
    printf(" -f, --file FILE   upload filepath (ignore when server)\n");
}

int parse_args(int argc, char *argv[]) {
    static char buf[50];
    int c = -1, tmp = 0, err = 0;
    while ((c = getopt_long(argc, argv, optstr, opts, NULL)) != -1) {
        switch (c) {
            case 'h': display_help(argv[0]); break;
            case 's': is_server = 1; break;
            case 'p': {
                err = copy_number(&tmp, optarg);
                if (err) return puts("Bad port number!"), -1;
                port = (uint16_t)tmp;
                break;
            }
            case 'q': {
                err = copy_number(&tmp, optarg);
                if (err) return puts("Bad queue list len!"), -1;
                queue = tmp;
                break;
            }
            case 'a': {
                err = copy_string(buf, optarg);
                if (err || !inet_aton(buf, &socket_addr.sin_addr)) return puts("Bad address!"), -1;
                set_addr = 1;
                break;
            }
            default: return -1;
        }
    }
    bzero(&socket_addr, sizeof(socket_addr));
    if (!set_addr) socket_addr.sin_addr.s_addr = INADDR_LOOPBACK; // 127.0.0.1
    socket_addr.sin_port = htons(port);
    socket_addr.sin_family = AF_INET;
    return 0;
}