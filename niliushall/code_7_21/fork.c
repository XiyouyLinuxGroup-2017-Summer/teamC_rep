/*************************************************************************
	> File Name: fork.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月21日 星期五 16时37分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
/*
int main (void)
{
    int pid;
    printf("进程创建:\n");
    pid = fork();

    switch(pid){
        case 0:
            printf("Child process is running, Curpid is %d. parentPid is %d\n", pid, getppid());
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
            printf("Parent process is running, ChildPid is %d. parentPid is %d\n", pid, getppid());
            break;
            
    }

    exit(0);
}
*/
/*
int main (void){
    int pid, k;
    char *str;

    printf("Process creation Study:\n");
    pid = fork();
    switch(pid){
        case 0:{
            str = "Child process is running\n";
            k = 3;
            break;
        }
        case -1:{
            perror("Process creation failed\n");
            break;
        }
        default:{
            str = "Parent prcoess is running\n";
            k = 5;
            break;
        }
    }

    while(k > 0){
        fputs(str, stdout);
        sleep(1);
        k--;
    }

    return 0;
}
*/
int main (void){
    pid_t pid;
    pid = fork();
    switch(pid){
        case 0:{
            printf("A background process, PID:%d, ParentID:%d\n", pid, getppid());
            sleep(3);
        }
        case -1:{
            perror("Process creation failed\n");
            exit(-1);
        }
        default:{
            printf("T am parent process, my pid is %d\n", getppid());
            exit(0);
        }
    }

    return 0;
}
