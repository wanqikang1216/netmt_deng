#ifndef __CAT_H__
#define __CAT_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

extern int my_cat(const char *file);


int my_cat(const char* file) {
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        perror("fail to open\n");
        return -1;
    }
    char buf[100];
    int n;
    while(1) {
        bzero(buf, sizeof(buf));
        n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            break;
        }
        printf("%s", buf);
    }
    close(fd);
    return 0;
}


#endif