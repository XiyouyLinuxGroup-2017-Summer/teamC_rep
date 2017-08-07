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

void err(const char *string, int line) {
	perror(string);
	printf("line : %d", line);
	exit(0);
}

int my_recv(int conn_fd, char *data_buf, int len) {
	static char recv_buf[BUFSIZE] = {0};
	static char *pread;
	static int len_remain = 0;
	int i;

	if(len_remain <= 0) {
		if((len_remain = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
			err("recv", __LINE__);
		else if(len_remain == 0)
			return 0;
		pread = recv_buf;
	}
printf("-%s-\n*%s*",recv_buf,pread);
	for(i = 0; *pread != '\n'; i++) {
// printf("-%c-\n", *pread);
		if(i > len){
// printf("i %d len %d\n", i, len);
			return -1;
		}
		data_buf[i] = *pread++;
		len_remain--;
	}

	len_remain--;
	pread++;

	return i;
}