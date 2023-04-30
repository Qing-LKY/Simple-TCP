#include "io.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>

int write_file(FILE *fp, const char *buf, int siz) {
    if (fwrite(buf, 1, siz, fp) < 0) {
        fprintf(stderr, "%d: Write file: %s\n", getpid(), strerror(errno));
        return -1;
    }
    return 0;
}

int send_content(int cfd, const char *buf, int siz) {
    if (send(cfd, buf, siz, 0) < 0) {
        fprintf(stderr, "%d: Send content: %s\n", getpid(), strerror(errno));
        return -1;
    }
    return 0;
}

int set_timeout(int cfd) {
    static struct timeval tv = {
        .tv_sec = 600, .tv_usec = 0
    };
    if (setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        fprintf(stderr, "%d: Set recv timeout: %s\n", getpid(), strerror(errno));
        return -1;
    }
    if (setsockopt(cfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
        fprintf(stderr, "%d: Set send timeout: %s\n", getpid(), strerror(errno));
        return -1;
    }
    return 0;
}