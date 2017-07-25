/*************************************************************************
	> File Name: daemon.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月22日 星期六 15时42分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>

int init_daemon(void) {
    int pid;
    int i;
    
    /*忽略终端I/O信号，STOP信号*/
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);


    pid = fork();

    if(pid > 0) {
        exit(0);        /*结束父进程，使得子进程成为后台进程*/
    } else if(pid < 0) {
        return -1;
    }

    /*创建一个新的进程组，在这个新的组中，子进程成为进程组的首进程，以使该进程脱离所有终端*/
    setsid();
    
    pid = fork();
    if(pid > 0) {
        exit(0);
    } else if(pid < 0) {
        return -1;
    }

    for(i = 0;i < NOFILE;close(i++));

    chdir("/");

    umask(0);

    signal(SIGCHLD,SIG_IGN);

    return 0;
}

int main(int argc, char *argv[]) {
    time_t now;
    init_daemon();
    syslog(LOG_USER|LOG_INFO,"测试守护进程!");
    while(1) {
        sleep(8);
        time(&now);
        syslog(LOG_USER|LOG_INFO,"系统时间：\t%s\t\t\n",ctime(&now));
    }

    return 0;
}

