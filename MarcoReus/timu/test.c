/*************************************************************************
	> File Name: test.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月29日 星期六 17时28分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int var = 1;

    pid = vfork();
    if(pid < 0) {
        return 0;
    }

    if(pid == 0) {
        var++;
        exit(0);
        //return 0;
    }
    
    printf("pid:%d  var:%d\n",pid,var);
    return 0;
}

