#include "order.h"

// #define MAX_OPTION      32
// // opt type
// #define NOPREFIX_OPTION 0
// #define SHORT_OPTION    -1
// #define LONG_OPTION     -2

// conf_ctx_t g_conf_ctx;

// inline void conf_ctx_init(conf_ctx_t* ctx) {
//     ctx->parser = new IniParser;
//     ctx->loglevel = LOG_LEVEL_DEBUG;
//     ctx->worker_processes = 0;
//     ctx->worker_threads = 0;
//     ctx->port = 0;
// }

// void print_version() {
//     printf("%s\n", default_version);
// }

// void print_help() {
//     printf("Usage: %s [%s]\n", g_main_ctx.program_name, options);
//     printf("Options:\n%s\n", detail_options);
// }

static const option_t* get_option(const char* opt, const option_t* long_options, int size) {
    if (opt == NULL || long_options == NULL) return NULL;
    int len = strlen(opt);
    if (len == 0)   return NULL;
    if (len == 1) {
        for (int i = 0; i < size; ++i) {
            if (long_options[i].short_opt == *opt) {
                return &long_options[i];
            }
        }
    } else {
        for (int i = 0; i < size; ++i) {
            if (strcmp(long_options[i].long_opt, opt) == 0) {
                return &long_options[i];
            }
        }
    }

    return NULL;
}

int parse_opt_long(int argc, char** argv, const option_t* long_options, int size) {
    char opt[MAX_OPTION+1] = {0};
    for (int i = 1; argv[i]; ++i) {
        char* arg = argv[i];
        int opt_type = NOPREFIX_OPTION;
        // prefix
        if (*arg == OPTION_PREFIX) {
            ++arg;
            opt_type = SHORT_OPTION;
            if (*arg == OPTION_PREFIX) {
                ++arg;
                opt_type = LONG_OPTION;
            }
        }
        int arg_len  = strlen(arg);
        // delim
        char* delim = strchr(arg, OPTION_DELIM);
        if (delim) {
            if (delim == arg || delim == arg+arg_len-1 || delim-arg > MAX_OPTION) {
                printf("Invalid option '%s'\n", argv[i]);
                return -10;
            }
            memcpy(opt, arg, delim-arg);
            opt[delim-arg] = '\0';
        } else {
            if (opt_type == SHORT_OPTION) {
                *opt = *arg;
                opt[1] = '\0';
            } else {
                strncpy(opt, arg, MAX_OPTION);
            }
        }
        // get_option
        const option_t* pOption = get_option(opt, long_options, size);
        if (pOption == NULL) {
            if (delim == NULL && opt_type == NOPREFIX_OPTION) {
                g_main_ctx.arg_list.push_back(arg);
                continue;
            } else {
                printf("Invalid option: '%s'\n", argv[i]);
                return -10;
            }
        }
        const char* value = NULL;
        if (pOption->arg_type == NO_ARGUMENT) {
            // -h
            value = OPTION_ENABLE;
        } else if (pOption->arg_type == REQUIRED_ARGUMENT) {
            if (delim) {
                // --port=80
                value = delim+1;
            } else {
                if (opt_type == SHORT_OPTION && *(arg+1) != '\0') {
                    // p80
                    value = arg+1;
                } else if (argv[i+1] != NULL) {
                    // --port 80
                    value = argv[++i];
                } else {
                    printf("Option '%s' requires parament\n", opt);
                    return -20;
                }
            }
        }
        // preferred to use short_opt as key
        if (pOption->short_opt > 0) {
            g_main_ctx.arg_kv[std::string(1, pOption->short_opt)] = value;
        } else if (pOption->long_opt) {
            g_main_ctx.arg_kv[pOption->long_opt] = value;
        }
    }
    return 0;
}

int order_main(int argc, char** argv) {
    main_ctx_init(argc, argv);

    if (argc == 1) {
        print_help();
        exit(10);
    }
    int ret = parse_opt_long(argc, argv, long_options, ARRAY_SIZE(long_options));
    if (ret != 0) {
        print_help();
        exit(ret);
    }

    if (get_arg("h")) {
        print_help();
        exit(0);
    }

    // version
    if (get_arg("v")) {
        print_version();
        exit(0);
    }

    // g_conf_ctx
    conf_ctx_init(&g_conf_ctx);
    const char* confile = get_arg("c");
    if (confile) {
        strncpy(g_main_ctx.confile, confile, sizeof(g_main_ctx.confile));
    }
    parse_confile(g_main_ctx.confile);

    // test
    if (get_arg("t")) {
        printf("Test confile [%s] OK!\n", g_main_ctx.confile);
        exit(0);
    }

    // signal
    signal_init(on_reload);
    const char* signal = get_arg("s");
    if (signal) {
        signal_handle(signal);
    }

#ifdef OS_UNIX
    // daemon
    if (get_arg("d")) {
        // nochdir, noclose
        int ret = daemon(1, 1);
        if (ret != 0) {
            printf("daemon error: %d\n", ret);
            exit(-10);
        }
    }
#endif

    //command,增加命令行功能
    if (get_arg("o")) {
        if (argc < 3) {
            printf("%s\n, usage:%s", "invalid command", command);
        }else if (argc == 3) {
            ls_dir(argc, argv);
        }else {
            if (argv[2] == "ls") {
                ls_dir(argc, argv);
            }
            if (argv[2] == "mkdir") {
                hv_mkdir_p(argv[3]);
            }
            if (argv[2] == "rmdir") {
                hv_rmdir_p(argv[3]);
            }
            if (argv[2] == "touch") {
                mytouch(argv[3]);
            }
            if (argv[2] == "cat") {
                my_cat(argv[3]);
            }
            if (argv[2] == "vim") {
                my_vim(argv[3]);
            }
        }
    }

    // pidfile
    create_pidfile();

    master_workers_run(worker_fn, (void*)(intptr_t)g_conf_ctx.port, g_conf_ctx.worker_processes, g_conf_ctx.worker_threads);

    return 0;
}

// void worker_fn(void* userdata) {
//     long port = (long)(intptr_t)(userdata);
//     while (1) {
//         printf("port=%ld pid=%ld tid=%ld\n", port, hv_getpid(), hv_gettid());
//         hv_delay(60000);
//     }
// }