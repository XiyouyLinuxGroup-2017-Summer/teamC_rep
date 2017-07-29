/*************************************************************************
	> File Name: guer.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月25日 星期二 08时09分27秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
    pid_t pid;
    pid = fork();
    int n = 5;
    switch(pid){
        case 0:
            while(n--){
                printf("当前为子进程 pid = %d,"
                "ppid = %d\n",getpid(),getppid());
                sleep(3);
            }
            break;
            case -1:
                printf("进程创建失败\n");
                exit(-1);
            default:
                printf("当前为父进程 pid = %d\n",getpid());
    }
    return 0;
}
