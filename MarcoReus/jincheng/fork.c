/*************************************************************************
	> File Name: fork.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月22日 星期六 09时36分01秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    pid_t pid;

    printf("Poccess Creation Study\n");
    pid = fork();
    
    switch(pid) {
        case 0:
            printf("Child process is running,CurPid is %d,ParentPid is %d\n",pid,getppid());
            break;
        case -1:
            printf("Parent process is running,ChildPid is %d,ParentPid is %d\n",pid,getpid());
            break;
        default:
            break;
    }
    
    return 0;
}

