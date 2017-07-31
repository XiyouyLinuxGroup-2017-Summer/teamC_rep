/*************************************************************************
	> File Name: test3.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月29日 星期六 17时53分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid_1;
    pid_t pid_2;

    pid_1 = fork();
    pid_2 = fork();

    printf("%d %d\n",pid_1,pid_2);

    return 0;
}

