/*************************************************************************
	> File Name: fork3.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月24日 星期一 11时00分38秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main (void)
{
    pid_t pid;
    int i = 3;
    pid = fork();
    switch(pid){
        case 0:
            while(i--){
                printf("A background process, PID:%d, ParentID:%d\n", getpid(), getppid());
            }
            break;

        case -1:
            perror("Process creation failed.\n");
            exit(1);

        default:
            printf("I am parent process, my pid is %d\n", getpid());
            exit(0);
    }

    return 0;
}
