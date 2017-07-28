/*************************************************************************
	> File Name: wait.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 18时02分53秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main (void)
{
    pid_t pid;
    char *msg;
    int k;
    int exit_code;

    printf("Study how to get exit code:\n");
    pid = fork();
    switch(pid){
        case 0:{
            msg = "Child process is runnning";
            k = 5;
            exit_code = 2;
            break;
        }
        case -1:{
            perror("Process creation failed.\n");
            exit(1);
        }
        default:{
            exit_code = 0;
            break;
        }
    }

    if(pid != 0){
        int stat_val;//子进程的退出码
        pid_t child_pid;//被关闭的子进程ID

        child_pid = wait(&stat_val);
        printf("Child process has exited, pid=%d\n", child_pid);
        if(WIFEXITED(stat_val))//子进程是否正常退出
            printf("Child process exited with code %d\n", WEXITSTATUS(stat_val));//获取子进程退出码，即子进程exit()的参数值
        else
            printf("Child process exited abnormally\n");
        
    }
    else{
        while(k--){
            puts(msg);
            sleep(1);
        }
    }
    //exit(exit_code);
    exit(6);//子进程退出码为6,则WEXITSTATUS()的返回值为6
}
