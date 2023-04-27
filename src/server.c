#include "args.h"
#include "server.h"
#include "io.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

int sfd = 0, cfd = 0;
struct sockaddr_in cli_addr;
socklen_t s_len;

void sig_handle_server(int signo) {
    char *ip;
    if (signo == SIGINT) {
        printf("Pid=%d terminated\n", getpid());
        if (cfd != 0) {
            ip = inet_ntoa(cli_addr.sin_addr);
            printf("Client:%s stop\n", ip);
        }
        exit(EXIT_SUCCESS);
    }
}

int server_prework() {
    int err;
    // socket for server
    sfd = socket(AF_INET, SOCK_STREAM, PF_INET);
    if (sfd < 0) {
        perror("Generate socket");
        return -1;
    }
    // bind address
    if (bind(sfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) == -1) {
        perror("Bind socket");
        return -1;
    }
    // set pending queue length
    if (listen(sfd, queue) == -1) {
        perror("Listen");
        return -1;
    }
    // set handler
    if (signal(SIGINT, sig_handle_server) == SIG_ERR) {
        perror("Set SIGINT handler");
        return -1;
    }
    return 0;
}

void server_listener() {
    pid_t pid;
    // main loop
    for (;;) {
        cfd = accept(sfd, (struct sockaddr *)&cli_addr, &s_len);
        if (cfd < 0) {
            perror("Accept");
            continue;
        }
        pid = fork();
        if (pid == -1) {
            perror("Fork");
            continue;
        }
        if (pid == 0) {
            close(sfd), sfd = 0;
            server_connection();
            exit(EXIT_SUCCESS);
        } else close(cfd), cfd = 0;
    }
}

char *get_logname() {
    static char path[100];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(path, sizeof(path), "%Y-%m-%d-%H:%M:%S.log", local);
    return path;
}

void server_connection() {
    int siz = 0, re = 0, tot = 0;
    char buf[1024];
    char *log_name;
    FILE *log_fp;
    // set timeout
    if (set_timeout(cfd) != 0) exit(EXIT_FAILURE);
    // open log
    log_name = get_logname();
    log_fp = fopen(log_name, "w");
    if (log_fp == NULL) {
        perror(log_name);
        exit(EXIT_FAILURE);
    }
    // basic message
    sprintf(buf, "Client IP: %s\n\n------\n\n", inet_ntoa(cli_addr.sin_addr));
    siz = strlen(buf);
    if (write_file(log_fp, buf, siz) != 0) exit(EXIT_FAILURE);
    if (send_content(cfd, buf, siz) != 0) exit(EXIT_FAILURE);
    // main loop
    for (;;) {
        siz = recv(cfd, buf, 1024, MSG_WAITALL);
        if (siz == 0) break; // normal disconnected
        if (siz == -1) {
            printf("%d: Recv: %s\n", getpid(), strerror(errno)); // timeout
            exit(EXIT_FAILURE);
        }
        if (write_file(log_fp, buf, siz) != 0) exit(EXIT_FAILURE);
        if (send_content(cfd, buf, siz) != 0) exit(EXIT_FAILURE);
        tot += siz;
    }
    // Bytes count
    fprintf(log_fp, "\n\n------\n\nTotal Length: %d\n", tot);
    sprintf(buf, "Total Length: %d\n", tot);
    if (write_file(log_fp, buf, siz) != 0) exit(EXIT_FAILURE);
    if (send_content(cfd, buf, siz) != 0) exit(EXIT_FAILURE);
    return;
}