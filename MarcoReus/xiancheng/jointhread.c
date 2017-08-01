/*************************************************************************
	> File Name: jointhread.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 16时39分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void assisthread(void *arg) {
    printf("I am helping to do some jobs\n");
    sleep(3);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t assistthid;
    int status;

    pthread_create(&assistthid,NULL,(void *)assisthread,NULL);
    pthread_join(assistthid,(void *)&status);
    printf("assistthread's exit is caused %d\n",status);

    return 0;
}


