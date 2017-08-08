#include "myhead.h"

#define INVALID_USERINFO 'n'
#define VALID_USERINFO   'y'

int get_userinfo(char *buf, int len) {
    int i;

    if(buf == NULL)
        return -1;

    i = 0;
    while((buf[i++] = getchar()) != '\n' && i < len-1)
        ;
    buf[i-1] = 0;
printf("+%s+\n", buf);
    
    return 0;
}

void input_userinfo(int conn_fd, char *string) {
    char recv_buf[ BUFSIZE ];
    char input_buf[ NAMESIZE ] = {0};
    int flag_userinfo = 0;

    do {
        printf("%s : ", string);
        if(get_userinfo(input_buf, NAMESIZE) < 0)
            err("error return from get_userinfo", __LINE__);
        
        if(send(conn_fd, input_buf, sizeof(input_buf), 0) < 0)
            err("send", __LINE__);

        /*读取套接字数据*/
        if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0)
            err("recv", __LINE__);

        if(recv_buf[0] == VALID_USERINFO) {
            flag_userinfo = VALID_USERINFO;
        } else {
            printf("%s error, input again\n", string);
            flag_userinfo = INVALID_USERINFO;
        }
    } while(flag_userinfo == INVALID_USERINFO);
}

int main(int argc, char **argv) {
    int conn_fd, serv_port;
    struct sockaddr_in serv_addr;
    char recv_buf[BUFSIZE];
    int i, ret;

    if(argc != 3)
        err("The number of argc", __LINE__);

    /*初始化服务器端地址结构*/
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);

    /*检查参数*/
    if(!strcmp(argv[1], "-a")) {
        if(inet_aton(argv[2], &serv_addr.sin_addr) < 0) {
            printf("invalid server ip address\n");
            exit(1);
        }
    } else {
        printf("there should be [ -a ] [ server_ip ]\n");
        exit(1);
    }

    /*创建socket*/
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
        err("socket", __LINE__);

    /*向服务器发送连接请求*/
    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
        err("connect", __LINE__);

    input_userinfo(conn_fd, "username");
    input_userinfo(conn_fd, "password");
// sleep(1);
    printf("aaaaa\n");
    memset(recv_buf,0,sizeof(recv_buf));
    


    if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {
        printf("data is too long\n");
        exit(1);
    }

    sleep(3);
    if(recv_buf != NULL)
        printf("%s\n", recv_buf);
    printf("ret = %d\n", ret);
    for(i = 0; i < ret; i++)
        printf("%c", recv_buf[i]);
    printf("\n");

    close(conn_fd);

    return 0;
}
