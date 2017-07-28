/*************************************************************************
	> File Name: execve.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月24日 星期一 15时09分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[],char **environ) {
    pid_t pid;
    int stat_val;

    printf("Exec example\n");
    pid = fork();
    switch(pid) {
        case -1:
            perror("Process Creation failed\n");
        case 0:
            printf("Child process is running\n");
            printf("My pid = %d,parentpid = %d\n",getpid(),getppid());
            printf("uid = %d,gid = %d\n",getuid(),getpid());
            execve("processimage",argv,environ);
            printf("process never go to here!\n");
            exit(0);
        default:
            printf("Parent Process is running\n");
            break;
    }
    
    wait(&stat_val);
    exit(0);
}

