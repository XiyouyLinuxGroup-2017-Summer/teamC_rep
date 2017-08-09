#include "myhead.h"

struct userinfo1 {
    char username[32];
    char password[32];
};
struct userinfo1 users[] = {{"linux", "unix"}, {"123", "234"}, {"clh", "clh"}, {" ", " "}};

int find_name(char *name) {
    int i;

    if(name == NULL) {
        printf("in find_nmae, NULL pointer\n");
        return -2;
    }
    for(i = 0; users[i].username[0] != ' '; i++)
        if(!strcmp(users[i].username, name))
            return i;
    return -1;
}

void send_data(int conn_fd, char *string) {
    
    if(send(conn_fd, string, strlen(string), 0) < 0 )
        err("send_data", __LINE__);
}

int main(void) {
    int fd;
    int sock_fd, conn_fd;
    int optval;
    int flag_recv = USERNAME;
    int ret, name_num;
    pid_t pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;
    char recv_buf[ BUFSIZE ];

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

    if(listen(sock_fd, LISTEN_SIZE) < 0)
        err("listen", __LINE__);

    cli_len = sizeof(struct sockaddr_in);
    while(1) {
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_len, &cli_len);
        if(conn_fd < 0)
            err("accept", __LINE__);

        printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));

        if((pid = fork()) == 0) {
            while(1) {
                if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
                    err("recv", __LINE__);
                recv_buf[ret-1] = 0;

                if(flag_recv == USERNAME) {
                    name_num = find_name(recv_buf);
                    switch(name_num){
                        case -1:
                            send_data(conn_fd, "n");
                            break;
                        case -2:
                            exit(1);
                        default:
                            send_data(conn_fd, "y\n");
                            flag_recv = PASSWORD;
                            break;
                    }
                } else if (flag_recv == PASSWORD) {
                    if(!strcmp(users[name_num].password, recv_buf)) {
                        send_data(conn_fd, "y\n");
                        printf("%s login\n", users[name_num].username);
                        break;
                    } else
                        send_data(conn_fd, "n");
                }
            }
            close(conn_fd);                       
            close(sock_fd);

            exit(0);
        } else {
            close(conn_fd);
        }
    }

    return 0;
}
