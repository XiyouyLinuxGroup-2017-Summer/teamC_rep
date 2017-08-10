#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CLEAR system("clear")

#define USER_INFO "/home/wangliang/chatroom_info/userinfo"
#define DIR_USER "/home/wangliang/chatroom_info/"
#define SERV_PORT 4507  //服务器端口号
#define GROUP_MEMBER 10 //群组最大用户数
#define NAMESIZE  21    //昵称最大长度
#define PASSWDSIZE 21   //密码最大长度
#define BUFSIZE 512     //最大传输字节数
#define LISTENSIZE 30

#define USERNAME 0      //接收到用户名
#define PASSWORD 1      //接收到密码

struct userinfo {   //记录用户信息
    char name[ NAMESIZE ];      //用户名
    int account;         //帐号
    char passwd[ PASSWDSIZE ];    // 密码
};

struct online_user {   // 记录在线用户信息
    int user_fd;    //用户socket
    char name[ NAMESIZE ];
    int account;
    struct online_user *next;
};

struct group {         // 记录群组信息
    char name[ GROUP_MEMBER ][ NAMESIZE ];
    int account[ GROUP_MEMBER ];
    char group_name[ NAMESIZE ];
};

struct message {
    char choice[5];
    int conn_fd;
};


void err(const char *, int ); // 错误处理
char *my_time();

void err(const char *string, int line) {
    perror(string);
    printf("line: %d\n", line);
    exit(1);
}

char *my_time() {
    time_t nowtime;
    nowtime = time(NULL);
    char *p = ctime(&nowtime);

    return p;
}