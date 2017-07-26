/*************************************************************************
	> File Name: fork3.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月22日 星期六 15时05分27秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    pid = fork();

    switch(pid) {
        case 0:
        while(1) {
            printf("A background ,PID:%d\n,ParentID:%d",getpid(),getppid());
            sleep(3);
        }
        case -1:
            perror("Pocess creation failed\n");
            exit(-1);
        default:
            printf("I am parent process,my pid is %d\n",getpid());
            exit(0);
    }

    return 0;
}

