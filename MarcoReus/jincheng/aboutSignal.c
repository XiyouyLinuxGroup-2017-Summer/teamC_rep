/*************************************************************************
	> File Name: aboutSignal.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 10时06分57秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    signal(SIGINT,SIG_IGN);
    
    for(;;) {
        printf("hahaha\n就是关不掉我\n");
    }

    return 0;
}

