//
// Created by dengxs on 2021/7/7 10:28.
//
#ifndef __PING_H__
#define __PING_H__

#include <stdbool.h>
#include <stdio.h>

#include "icmp.h"
#include "hlog.h"
#include "hfile.h"
#include "hmain.h"
#include "hbase.h"

// #define FATAL "../out/log/fatal.txt"

main_ctx_t g_main_ctx;

bool _Ping(int argc, char** argv, const char *host, int ping_cnt) {
    logger_t *logger;
    // const char *filepath = FATAL;
    // HFile file;
    logger = logger_create();
    int ok_cnt = ping(host, ping_cnt);
    if (ok_cnt < 0) {
        // printf("ping fatal!");
        // if (file.open(filepath.c_str(), "wb") != 0) {
        //     fprintf(stderr, "Failed to open file %s\n", filepath.c_str());
        //     return -20;
        // }
        // //获取运行程序的路径
        // get_run_dir(g_main_ctx.run_dir, sizeof(g_main_ctx.run_dir));
        // strncpy(g_main_ctx.program_name, hv_basename("/../out/bin/"), sizeof(g_main_ctx.program_name));
        // char logdir[MAX_PATH] = {0};
        // //日志存放路径
        // snprintf(logdir, sizeof(logdir), "%s/../out/logs", g_main_ctx.run_dir);
        // //创建日志目录
        // hv_mkdir(logdir);
        // snprintf(g_main_ctx.logfile, sizeof(g_main_ctx.logfile), "%s/../out/logs/%s.log", g_main_ctx.run_dir, g_main_ctx.program_name);
        // hlog_set_level_by_str("FATAL");
        // hlog_set_file(g_main_ctx.logfile);
        main_ctx_init(argc, argv);
        // hlog_set_file(filepath);
        hlogf("ping fatal!");
        return false;
    }else {
        printf("ping %d count, %d ok.\n", ping_cnt, ok_cnt);
        return true;
    }
}

#endif