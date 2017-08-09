#include "myhead.h"

struct userinfo1 {
    char username[32];
    char password[32];
};
//struct userinfo1 users[] = {{"linux", "unix"}, {"123", "234"}, {"clh", "clh"}, {" ", " "}};

int find_name(int account, char *passwd) {
    int i, flag = 0;
    FILE *fp = NULL;
    struct userinfo tmp;

    if((fp = fopen("userinfo", "r")) == NULL)
        err("fopen", __LINE__);

    while((fscanf(fp, "%s%d%s", tmp.name, &tmp.account, tmp.passwd) != EOF) && (tmp.account != account))
        ;
    if(tmp.account != account)
        return -1;
    strcpy(passwd, tmp.passwd);

    return 0;
}

void send_data(int conn_fd, char *string) {
    if(send(conn_fd, string, strlen(string), 0) < 0)
    err("send", __LINE__);
}

void *service(void *arg) {
    int ret, n, account;
    int conn_fd = *(int *) arg;
    int choice;
    int flag_recv = 0;
    char recv_buf[ BUFSIZE ];

    do{
        if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0){  //接收choice

            err("recv", __LINE__);
        }
        recv_buf[ret-1] = 0;
        choice = atoi(recv_buf);

        if(send(conn_fd, "y", 2, 0) < 0)
            err("send", __LINE__);

        switch(choice) {
            case 1: {     //登录
                char passwd[21];
                while(1){
                    if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
                        err("recv", __LINE__);
                    recv_buf[ret-1] = 0;

                    if(flag_recv == USERNAME) {
                        account = atoi(recv_buf);

                        n = find_name(account, passwd);

                        if(n == -1)
                            send_data(conn_fd, "n");
                        else {
                            send_data(conn_fd, "y\n");
                            flag_recv = PASSWORD;
                        }
                    } else if (flag_recv == PASSWORD) {
                        if(!strcmp(passwd, recv_buf)) {
                            send_data(conn_fd, "y\n");
                            printf("%d login\n", account);
                            break;
                        } else
                            send_data(conn_fd, "n");
                    }
                }
            }
            break;

            case 2: {

            }
            break;
        }
    } while(choice);
}


int main(void) {
    int fd, ret;
    int sock_fd, conn_fd;
    int optval;
    int flag_recv = USERNAME;
    pthread_t tid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;
    char recv_buf[5]; //接收choice

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
        err("socket", __LINE__);

    optval = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
        err("setsockopt", __LINE__);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)
        err("bind", __LINE__);

    if(listen(sock_fd, LISTENSIZE))

    cli_len = sizeof(struct sockaddr_in);
    while(1) {
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
        if(conn_fd < 0)
            err("accept", __LINE__);

        printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));
        
        if(pthread_create(&tid, NULL, service, (void *)&conn_fd) < 0) {
            err("pthread_create", __LINE__);
        }
    }

    return 0;
}
