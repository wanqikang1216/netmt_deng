/*
 * tcp echo server
 *
 * @build   make examples
 * @server  bin/tcp_echo_server 1234
 * @client  bin/nc 127.0.0.1 1234
 *          nc     127.0.0.1 1234
 *          telnet 127.0.0.1 1234
 */

#include "hsocket.h"
#include "hevent.h"
#include "hloop.h"
#include "string_ini.h"

// for stdin
// hio_t* clientio = NULL;
// for socket
hio_t* sockio = NULL;
//客户端的结构
typedef struct client_s {
    hloop_t* loop;
    hio_t*   listenio;
    int id;
    struct list_node clients;
} client_t;
//服务器主io
typedef struct io_s {
    hio_t* io;
    char addr[SOCKADDR_STRLEN];
    struct list_node node;
} io_t;

static client_t t_client;
// static io_t t_io;
static hmutex_t s_mutex;

//展示客户端连接的FD
static void ListFD() {
    // printf("客户端连接的FD:");
    struct list_node *node;
    io_t *conn;
    //遍历整个链表
    list_for_each(node, &t_client.clients) {
        conn = list_entry(node, io_t, node);
        printf("%d       %s\n", conn->io->fd, conn->addr);
    }
}

//向指定客户端发送命令，群发
//参数可变，传入客户端io的FD
static void SendStr(char* str) {
    struct list_node *node;
    io_t *conn;
    list_for_each(node, &t_client.clients) {
        conn = list_entry(node, io_t, node);
        hio_write(conn->io, str, strlen(str));
    }
}

static void on_close(hio_t* io) {
    printf("on_close fd=%d error=%d\n", hio_fd(io), hio_error(io));

    io_t *client = (io_t *)hevent_userdata(io);
    if (client) {
        hevent_set_userdata(io, NULL);

        hmutex_lock(&s_mutex);
        list_del(&client->node);
        hmutex_unlock(&s_mutex);

        HV_FREE(client);
    }
}

//写操作
static void on_send(hio_t* io, const void* buf, int readbytes) {
    // printf("on_stdin fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    // printf("> %s\n", (char*)buf);

    char* str = (char*)buf;

    //判断输入命令的长度
    int num = commandNum(str, splitstr);
    char **command = strsplit(str, splitstr);
    if (num == 1) {
        if (strncmp(str, "start", 5) == 0) {
            printf("call hio_read_start\n");
            hio_read_start(io);
            return;
        }
        else if (strncmp(str, "stop", 4) == 0) {
            printf("call hio_read_stop\n");
            hio_read_stop(io);
            return;
        }
        else if (strncmp(str, "close", 5) == 0) {
            printf("call hio_close\n");
            hio_close(io);
            return;
        }
        else if (strncmp(str, "quit", 4) == 0) {
            printf("call hloop_stop\n");
            hloop_stop(hevent_loop(io));
            return;
        }
        //服务器主动查看
        else if (strncmp(str, "PING", 4) == 0) {
            // hio_write(io, str, strlen(str));
            SendStr(str);
        }
        //查看连接客户端的FD
        else if (strncmp(str, "list", 4) == 0) {
            printf("客户端的FD:\n");
            ListFD();
            return;
        }
        else {
            printf("command invalid\n");
            return;
        }

    }
    else if (num == 2) {
        if (strncmp(command[0], "send", 4) == 0) {
            SendStr(command[1]);
        }
    }
    else {
        // CR|LF => CRLF for test most protocols
        char eol = str[readbytes-1];
        if (eol == '\n' || eol == '\r') {
            if (readbytes > 1 && str[readbytes-2] == '\r' && eol == '\n') {
                // have been CRLF
            }
            else {
                ++readbytes;
                str[readbytes - 2] = '\r';
                str[readbytes - 1] = '\n';
            }
        }
        hio_write(io, buf, readbytes);
    }
}

//读操作
static void on_recv(hio_t* io, void* buf, int readbytes) {
    printf("on_recv fd=%d readbytes=%d\n", hio_fd(io), readbytes);
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("[%s] <=> [%s]\n",
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));
    // printf("< %.*s", readbytes, (char*)buf);
    // echo

    printf("> %.*s", readbytes, (char*)buf);

    // fflush(stdout);
    //把接收到的数据不进行处理返回给客户端
    // char *test = "command invalid\n";
    // readbytes = strlen(test);
    char* str = (char*)buf;

    char eol = str[readbytes-1];
    if (eol == '\n' || eol == '\r') {
        if (readbytes > 1 && str[readbytes-2] == '\r' && eol == '\n') {
            // have been CRLF
        }
        else {
            ++readbytes;
            str[readbytes - 2] = '\r';
            str[readbytes - 1] = '\n';
        }
    }

    if (strncmp(str, "PING", 4) == 0) {
        char* strback = "PONG\r\n";
        hio_write(io, strback, strlen(strback));
    }
    // hio_write(io, test, readbytes);
    // on_stdin(io, buf, readbytes);
}


static void on_accept(hio_t* io) {
    printf("on_accept connfd=%d\n", hio_fd(io));

    // clientio[fd] = *io;
    // fd++;
    // if (fd >= MAXIO) {
    //     return -10;
    // }
    char localaddrstr[SOCKADDR_STRLEN] = {0};
    char peeraddrstr[SOCKADDR_STRLEN] = {0};
    printf("accept connfd=%d [%s] <= [%s]\n", hio_fd(io),
            SOCKADDR_STR(hio_localaddr(io), localaddrstr),
            SOCKADDR_STR(hio_peeraddr(io), peeraddrstr));

    hio_setcb_close(io, on_close);
    hio_setcb_read(io, on_recv);
    // hio_set_readbuf(io, recvbuf, RECV_BUFSIZE);
    // hio_setcb_write(io, on_send);
    // hio_read(io);
    hio_read_start(io);

    io_t *conn = NULL;
    HV_ALLOC_SIZEOF(conn);
    conn->io = io;
    strcpy(conn->addr, peeraddrstr); //存放客户端的地址
    hevent_set_userdata(io, conn);

    hmutex_lock(&s_mutex);
    list_add(&conn->node, &t_client.clients);
    hmutex_unlock(&s_mutex);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: ip port\n");
        return -10;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    // int i = 0;
    // int listenfd = 0;
    // int connfd = 0;

    hloop_t* loop = hloop_new(HLOOP_FLAG_QUIT_WHEN_NO_ACTIVE_EVENTS);
    // hloop_t* loop = hloop_new(0);
    // listenfd = Listen(port, ip);
    // connfd = ConnectNonblock(ip, port);
    // sockio = haccept(loop, listenfd, on_accept);


    hio_t* listenio = hloop_create_tcp_server(loop, ip, port, on_accept);

    if (listenio == NULL) {
        return -20;
    }
    printf("listenfd=%d\n", hio_fd(listenio));
    // hio_setcb_close(listenio, on_close);
    // // hio_setcb_write(io, on_stdin);
    // hio_setcb_read(listenio, on_recv);
    // hio_set_readbuf(listenio, recvbuf, RECV_BUFSIZE);
    // stdin use default readbuf
    t_client.loop = loop;
    t_client.listenio = listenio;
    srand(time(NULL));
    t_client.id = rand() % 1000000;
    list_init(&t_client.clients);
    hmutex_init(&s_mutex);

    hio_t* stdinio = hread(loop, 0, NULL, 0, on_send);
    if (stdinio == NULL) {
        return -20;
    }

    hloop_run(loop);
    hloop_free(&loop);
    return 0;
}