#include "args.h"
#include "io.h"
#include "client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int cfd, fd;

void sig_handle_client(int signo) {
    char *ip;
    if (signo == SIGINT) {
        puts("");
        printf("Pid=%d terminated\n", getpid());
        exit(EXIT_SUCCESS);
    }
}

int client_prework() {
    // set handler
    if (signal(SIGINT, sig_handle_client) == SIG_ERR) {
        perror("Set SIGINT handler");
        return -1;
    }
    // check filepath
    if (upload_file[0] == 0) {
        printf("Need upload filepath in client mode!\n");
        printf("Use -f to set it!\n");
        return -1;
    }
    fd = open(upload_file, O_RDONLY);
    if (fd < 0) {
        perror(upload_file);
        return -1;
    }
    // connect
    return client_connect();
}

int client_connect() {
    int re = 0;
    cfd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);
    if (cfd < 0) {
        perror("Generate socket");
        return -1;
    }
    // set timeout
    if (set_timeout(cfd) != 0) exit(EXIT_FAILURE);
    re = connect(cfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr));
    if (re < 0) {
        perror("Connect socket");
        return -1;
    }
    return 0;
}

int client_upload() {
    static char buf[1024];
    int siz = 0;
    while((siz = read(fd, buf, 1024)) > 0) {
        if (send_content(cfd, buf, siz) != 0) return -1;
        siz = recv(cfd, buf, 1024, 0);
        if (siz == 0) {
            printf("Disconnect with server!\n");
            return -1;
        }
        if (siz < 0) {
            printf("Recv: %s\n", strerror(errno));
            return -1;
        }
        if (write_file(stdout, buf, siz) != 0) return -1;
    }
    if (siz < 0) 
        return perror("Read"), -1;
    return 0;
}

int client_recv() {
    static char buf[1024];
    int siz = 0;
    for (;;) {
        siz = recv(cfd, buf, 1024, 0);
        if (siz == 0) break; // normal disconnected
        if (siz == -1) {
            printf("Recv: %s\n", strerror(errno)); // timeout
            return -1;
        }
        if (write_file(stdout, buf, siz) != 0) return -1;
    }
    return 0;
}