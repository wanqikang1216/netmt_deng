#ifndef __ORDER_H__
#define __ORDER_H__

#include "hv.h"
#include "hmain.h"
#include "iniparser.h"
#include "touch.h"
#include "cat.h"
#include "vim.h"

#define MAX_OPTION      32
// opt type
#define NOPREFIX_OPTION 0
#define SHORT_OPTION    -1
#define LONG_OPTION     -2

typedef struct conf_ctx_s {
    IniParser* parser;
    int loglevel;
    int worker_processes;
    int worker_threads;
    int port;
} conf_ctx_t;
conf_ctx_t g_conf_ctx;

//版本
char *default_version = 
"\r\nHello, this is Tool(version V0.0.1).\r\nCopyright 2021-2022 .\r\n";

// short options
static const char options[] = "hvc:ts:dp:o";

static const option_t long_options[] = {
    {'h', "help",       NO_ARGUMENT},
    {'v', "version",    NO_ARGUMENT},
    {'c', "confile",    REQUIRED_ARGUMENT},
    {'t', "test",       NO_ARGUMENT},
    {'s', "signal",     REQUIRED_ARGUMENT},
    {'d', "daemon",     NO_ARGUMENT},
    {'p', "port",       REQUIRED_ARGUMENT},
    {'o', "command",    OPTIONAL_ARGUMENT}
};

static const char detail_options[] = R"(
  -h|--help                 Print this information
  -v|--version              Print version
  -c|--confile <confile>    Set configure file, default etc/{program}.conf
  -t|--test                 Test configure file and exit
  -s|--signal <signal>      Send <signal> to process,
                            <signal>=[start,stop,restart,status,reload]
  -d|--daemon               Daemonize
  -p|--port <port>          Set listen port
  -o|--basic command        common command: ls, mkdir, rm, touch
)";

static const char command[] = R"(
    Usage:
        -o ls
           mkdir
           rmdir
           touch
           cat
           vim
)";

// #define MAX_OPTION      32
// // opt type
// #define NOPREFIX_OPTION 0
// #define SHORT_OPTION    -1
// #define LONG_OPTION     -2

inline void conf_ctx_init(conf_ctx_t *ctx);
void print_version();
void print_help();
static const option_t *get_option(const char *opt, const option_t *long_options, int size);
int parse_opt_long(int argc, char **argv, const option_t *long_options, int size);
int parse_confile(const char *confile);
static void on_reload(void *userdata);
//列出当前目录或指定目录下的所有文件和目录
int ls_dir(int argc, char **argv);
extern int order_main(int argc, char **argv);
void worker_fn(void *userdata);

inline void conf_ctx_init(conf_ctx_t* ctx) {
    ctx->parser = new IniParser;
    ctx->loglevel = LOG_LEVEL_DEBUG;
    ctx->worker_processes = 0;
    ctx->worker_threads = 0;
    ctx->port = 0;
}

void print_version() {
    printf("%s\n", default_version);
}

void print_help() {
    printf("Usage: %s [%s]\n", g_main_ctx.program_name, options);
    printf("Options:\n%s\n", detail_options);
}

int parse_confile(const char* confile) {
    int ret = g_conf_ctx.parser->LoadFromFile(confile);
    if (ret != 0) {
        printf("Load confile [%s] failed: %d\n", confile, ret);
        exit(-40);
    }

    // logfile
    string str = g_conf_ctx.parser->GetValue("logfile");
    if (!str.empty()) {
        strncpy(g_main_ctx.logfile, str.c_str(), sizeof(g_main_ctx.logfile));
    }
    hlog_set_file(g_main_ctx.logfile);
    // loglevel
    str = g_conf_ctx.parser->GetValue("loglevel");
    if (!str.empty()) {
        hlog_set_level_by_str(str.c_str());
    }
    // log_filesize
    str = g_conf_ctx.parser->GetValue("log_filesize");
    if (!str.empty()) {
        hlog_set_max_filesize_by_str(str.c_str());
    }
    // log_remain_days
    str = g_conf_ctx.parser->GetValue("log_remain_days");
    if (!str.empty()) {
        hlog_set_remain_days(atoi(str.c_str()));
    }
    // log_fsync
    str = g_conf_ctx.parser->GetValue("log_fsync");
    if (!str.empty()) {
        logger_enable_fsync(hlog, getboolean(str.c_str()));
    }
    // first log here
    hlogi("%s", default_version);
    hlog_fsync();

    // worker_processes
    int worker_processes = 0;
    str = g_conf_ctx.parser->GetValue("worker_processes");
    if (str.size() != 0) {
        if (strcmp(str.c_str(), "auto") == 0) {
            worker_processes = get_ncpu();
            hlogd("worker_processes=ncpu=%d", worker_processes);
        }
        else {
            worker_processes = atoi(str.c_str());
        }
    }
    g_conf_ctx.worker_processes = LIMIT(0, worker_processes, MAXNUM_WORKER_PROCESSES);
    // worker_threads
    int worker_threads = g_conf_ctx.parser->Get<int>("worker_threads");
    g_conf_ctx.worker_threads = LIMIT(0, worker_threads, 16);

    // port
    int port = 0;
    const char* szPort = get_arg("p");
    if (szPort) {
        port = atoi(szPort);
    }
    if (port == 0) {
        port = g_conf_ctx.parser->Get<int>("port");
    }
    if (port == 0) {
        printf("Please config listen port!\n");
        exit(-10);
    }
    g_conf_ctx.port = port;

    hlogi("parse_confile('%s') OK", confile);
    return 0;
}

static void on_reload(void* userdata) {
    hlogi("reload confile [%s]", g_main_ctx.confile);
    parse_confile(g_main_ctx.confile);
}

//列出当前目录或指定目录下的所有文件和目录
int ls_dir(int argc, char** argv) {
    const char* dir = ".";
    if (argc > 3) {
        dir = argv[3];
    }
    std::list<hdir_t> dirs;
    listdir(dir, dirs);
    for (auto& item : dirs) {
        printf("%c%c%c%c%c%c%c%c%c%c\t",
            item.type,
            item.mode & 0400 ? 'r' : '-',
            item.mode & 0200 ? 'w' : '-',
            item.mode & 0100 ? 'x' : '-',
            item.mode & 0040 ? 'r' : '-',
            item.mode & 0020 ? 'w' : '-',
            item.mode & 0010 ? 'x' : '-',
            item.mode & 0004 ? 'r' : '-',
            item.mode & 0002 ? 'w' : '-',
            item.mode & 0001 ? 'x' : '-');
        float hsize;
        if (item.size < 1024) {
            printf("%lu\t", item.size);
        }
        else if ((hsize = item.size/1024.0f) < 1024.0f) {
            printf("%.1fK\t", hsize);
        }
        else if ((hsize /= 1024.0f) < 1024.0f) {
            printf("%.1fM\t", hsize);
        }
        else {
            hsize /= 1024.0f;
            printf("%.1fG\t", hsize);
        }
        struct tm* tm = localtime(&item.mtime);
        printf("%04d-%02d-%02d %02d:%02d:%02d\t",
                tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        printf("%s%s\n", item.name, item.type == 'd' ? "/" : "");
    }
    return 0;
}

void worker_fn(void* userdata) {
    long port = (long)(intptr_t)(userdata);
    while (1) {
        printf("port=%ld pid=%ld tid=%ld\n", port, hv_getpid(), hv_gettid());
        hv_delay(60000);
    }
}

#endif