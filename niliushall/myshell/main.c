/*************************************************************************
	> File Name: main.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 14时51分03秒
 ************************************************************************/

#include "myshell.h"

void print_prompt(void){
    printf("myshell$$ ");
}

int main (int argc, char **argv)
{
    int i;
    int argcount = 0;//待执行命令的参数个数
    char arglist[100][256] = {0};
    char **arg = NULL;
    char *buf = NULL;//存放输入命令

    buf = (char *)malloc(256);
    if(buf == NULL){
        perror("malloc failed");
        exit(1);
    }
    while(1){
        memset(buf, 0, 256);//将buf清零
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
