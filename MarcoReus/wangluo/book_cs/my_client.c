/*************************************************************************
	> File Name: my_client.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 17时42分53秒
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

#define BUFSIZE 1024
#define INVALID_USERINFO 'n'
#define VALID_USERINFO   'y'

int my_recv(int conn_fd, char *data_buf, int len);
void err(const char *string, int line);

int get_userinfo(char *buf, int len) {
	int i = 0, c;

	if(buf == NULL)
		return -1;

	while(((c = getchar()) != '\n' && c != EOF && i < len-2))
		buf[i++] = c;
	buf[i++] = '\n';
	buf[i++] = 0;

	return 0;
}

void input_userinfo(int conn_fd, const char *string) {
	char input_buf[32];
	char recv_buf[BUFSIZE];
	int flag_userinfo;

	do {
		printf("%s: ", string);
		if(get_userinfo(input_buf, 32) < 0)
			err("fetinfo", __LINE__);

		if(send(conn_fd, input_buf, strlen(input_buf), 0) < 0)
			err("send", __LINE__);

		if(my_recv(conn_fd, recv_buf, sizeof(recv_buf)) < 0){
// printf("**%s**\n", recv_buf);
			err("my_recv", __LINE__);
		}

		if(recv_buf[0] == VALID_USERINFO)
			flag_userinfo = VALID_USERINFO;
		else {
			printf("%s error, input again", string);
			flag_userinfo = INVALID_USERINFO;
		}
	}while(flag_userinfo == INVALID_USERINFO);
}

int main(int argc, char **argv) {
	int i, ret, conn_fd, serv_port;
	struct sockaddr_in serv_addr;
	char recv_buf[BUFSIZE];

	if(argc != 5)
		err("argc", __LINE__);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	for(i = 1; i < argc; i++) {
		if(!strcmp("-p", argv[i])) {
			serv_port = atoi(argv[i + 1]);
			if(serv_port < 0 || serv_port > 65535)
				err("serv_port", __LINE__);
			else
				serv_addr.sin_port = htons(serv_port);
		}
		else if(!strcmp("-a", argv[i])) {
			if(inet_aton(argv[i+1], &serv_addr.sin_addr) == 0)
				err("inet_aton", __LINE__);
		}
	}

	if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0)
		err("argc", __LINE__);

	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd < 0)
		err("socket", __LINE__);

	if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
		err("connect", __LINE__);

	input_userinfo(conn_fd, "userame");
	input_userinfo(conn_fd, "password");

	if((ret = my_recv(conn_fd, recv_buf, sizeof(recv_buf))) < 0)
		err("my_recv", __LINE__);

	for(i = 0; i < ret; i++)
		printf("%c", recv_buf[i]);
	printf("\n");

	close(conn_fd);

	return 0;
}
