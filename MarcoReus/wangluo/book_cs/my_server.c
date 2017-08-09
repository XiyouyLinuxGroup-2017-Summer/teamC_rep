/*************************************************************************
	> File Name: my_server.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月08日 星期二 19时06分40秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SERV_PORT 4508
#define LISTENQ 12
#define INVALID_USERINFO 'n'
#define VALID_USERINFO 'y'
#define USERNAME 0
#define PASSWORD 1

struct userinfo {
	char username[32];
	char password[32];
};

struct userinfo users[] = {{"linux", "unix"}, {"4507", "4508"}, {"clh", "clh"}, {" ", " "}};

void err(const char *string, int line) {
	perror(string);
	printf("line : %d", line);
	exit(0);
}

int find_name(const char *name) {
	int i;
	if(name == NULL){
		printf("in find_name, NULL pointer\n");
		return -2;
	}

	for(i = 0; users[i].username[0] != 0; i++)
		if(!strcmp(users[i].username, name))
			return i;
	return -1;
}

void send_data(int conn_fd, const char *string) {
	if(send(conn_fd, string, sizeof(string), 0) < 0)
		err("send", __LINE__);
}

int main (void) {
	int sock_fd, conn_fd;
	int optval, ret, pid, cli_len, name_num;
	int flag_recv = USERNAME;
	struct sockaddr_in cli_addr, serv_addr;
	char recv_buf[128];

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

	if(listen(sock_fd, LISTENQ) < 0)
		err("listen", __LINE__);

	cli_len = sizeof(struct sockaddr_in);
	while(1) {
		conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
		if(conn_fd < 0)
			err("accept", __LINE__);

		printf("accept a new client, ip: %s\n", inet_ntoa(cli_addr.sin_addr));

		if((pid = fork()) == 0) {
			while(1) {
				if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
					err("recv", __LINE__);
				recv_buf[ret - 1] = 0;

				if(flag_recv == USERNAME) {
					name_num = find_name(recv_buf);
					switch(name_num) {
						case -1:
							send_data(conn_fd, "n\n");
							break;
						case -2:
							exit(1);
						default:
							send_data(conn_fd, "y\n");
							flag_recv = PASSWORD;
							break;
					}
				}
				else if(flag_recv == PASSWORD) {
					if(!strcmp(users[name_num].password, recv_buf)) {
						send_data(conn_fd, "y\n");
						send_data(conn_fd, "Welcome to my tcp server.\n");
						printf("%s login\n", users[name_num].username);
						break;
					}
					else
						send_data(conn_fd, "n\n");
				}
			}
			close(sock_fd);
			close(conn_fd);
			exit(0);
		}
		else
			close(conn_fd);
	}

	return 0;
}
