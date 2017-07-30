/*************************************************************************
	> File Name: test2.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月29日 星期六 17时41分27秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    for(int i = 0;i < 2;i++) {
        fork();
        printf("-");
    }
    wait(NULL);
    wait(NULL);

    return 0;
}

