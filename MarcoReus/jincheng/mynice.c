/*************************************************************************
	> File Name: mynice.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月25日 星期二 11时06分52秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int start_val = 0;
    int oldpri;
    int newpri;

    printf("nice study\n");

    pid = fork();

    switch(pid) {
        case 0:
            printf("Child is running,CurPid is %d,ParentPid is %d\n",pid,getppid());
            
            oldpri = getpriority(PRIO_PROCESS,0);
            printf("Old priority = %d\n",oldpri);

            newpri = nice(2);
            printf("New priority = %d\n",newpri);
            exit(0);
        case -1:
            perror("Process creation failed\n");
        default:
            printf("Parent is running,ChildPid is %d,ParentPid is %d\n",pid,getpid());
        break;
    }

    wait(&start_val);
    exit(0);
}

