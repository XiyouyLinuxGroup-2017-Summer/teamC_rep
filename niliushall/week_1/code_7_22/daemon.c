/*************************************************************************
	> File Name: daemon.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 10时58分16秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <signal.h>
#include <syslog.h>
#include <time.h>

int init_daemon(void){
    int pid;
    int i;

    /*忽略终端I/O信号,STOP信号*/
    signal(SIGTTOU, SIG_IGN); //Background Group的进程尝试写Terminal时
    signal(SIGTTIN, SIG_IGN); //Background Group的进程尝试读取Terminal时
    signal(SIGTSTP, SIG_IGN); //Suspend Key，一般是Ctrl+Z, 发送给所有Foreground Group的进程
    signal(SIGHUP, SIG_IGN);  //发送给具有Terminal的Controlling Process，当terminal被disconnect时

    pid = fork();
    if(pid > 0)
        exit(0);  //结束父进程,成为后台进程
    else if(pid < 0)
        return -1;

    setsid();//新建进程组，脱离终端控制

    /*新建进程，退出父进程，不再是进程组长，使其无法打开终端*/
    pid = fork();
    if(pid > 0)
        exit(0);
    else if(pid < 0)
        return -1;

    /*关闭父进程无用文件描述符*/
    for(i = 0; i < NOFILE; close(i++))
        ;

    chdir("/");//改变工作目录

    umask(0);//将文件屏蔽字设为0

    signal(SIGCHLD, SIG_IGN);//忽略SIGCHLD信号
    return 0;
}
int main (void)
{
    time_t now;
    init_daemon();
    syslog(LOG_USER|LOG_INFO, "测试守护进程！\n");
    while(1){
        sleep(8);
        time(&now);
        syslog(LOG_USER|LOG_INFO, "系统时间: \t%s\t\t\n", ctime(&now));
    }

    return 0;
}
