#ifndef __VIM_H__
#define __VIM_H__

//模仿vi实现基本的编辑功能
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1024

extern int my_vim(const char *file);

int my_vim(const char* file) {
    // if (argc < 2) {
    //     //没有命令行的参数
    //     return -1;
    // }
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        printf("创建文件失败\n");
    }
    char str[LEN];
    while (1) {

        // gets_s(str);
        // getline(str);
        // getchar();
        fgets(str, LEN, stdin);
        if (str[LEN - 1] == '\n') {
            str[LEN - 1] = '\0';
        }
        if (strncmp(str, "exit", 4) == 0) {
            break;
        }
        fprintf(fp, "%s\n", str);
        memset(str, 0, 1024);
    }
    fclose(fp);
    return 0;
}

#endif