#ifndef IO_H
#define IO_H

#include <stdio.h>

int write_file(FILE *fp, const char *buf, int siz);
int send_content(int cfd, const char *buf, int siz);
int set_timeout(int cfd);

#endif