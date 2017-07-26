/*************************************************************************
	> File Name: main.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 14时51分03秒
 ************************************************************************/

#include "myshell.h"

void print_prompt(void){
    char buf[256];
    printf("\033[1;32mmyshell\033[0m:\033[1;34m%s\033[0m$ ", getcwd(buf, 256));
}

int main (int argc, char **argv)
{
    int i;
    int argcount = 0;//待执行命令的参数个数
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;//存放输入命令

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    buf = (char *)malloc(256);
    if(buf == NULL){
        perror("malloc failed");
        exit(1);
    }
    while(1){
        memset(buf, 0, 256);//将buf清零  
        memset(arglist, 0, 25600);  
        print_prompt();
        get_input(buf);

        if(!strcmp(buf, "exit") || !strcmp(buf, "logout"))
            break;

        argcount = explain_input(buf, arglist);

        do_cmd(argcount, arglist);
    }

    if(buf != NULL){
        free(buf);
        buf = NULL;
    }

    return 0;
}