/*************************************************************************
	> File Name: create_socket.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 10时52分29秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    int socket_fd;

    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd < 0) {
        perror("socket");
        exit(1);
    }

    return 0;
}

