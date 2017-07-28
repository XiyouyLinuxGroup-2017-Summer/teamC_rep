/*************************************************************************
	> File Name: getpid.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月25日 星期二 10时15分57秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    if((pid = fork() == -1)) {
        printf("fork error!\n");
        exit(0);
    }

    if(pid == 0) {
        printf("getpid return %d\n",getpid());
    }

    exit(0);
}

