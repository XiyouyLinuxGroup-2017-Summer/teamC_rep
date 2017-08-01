/*************************************************************************
	> File Name: oncerun.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 11时19分04秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void) {
    printf("Function run is running in thread %u\n",pthread_self());
}

void *thread1(void *arg) {
    pthread_t thid = pthread_self();

    printf("Current thread's ID is %u\n",thid);
    pthread_once(&once,run);
    printf("thread1 ends\n");
}

void *thread2(void *arg) {
    pthread_t thid = pthread_self();

    printf("Current thread's ID is %u\n",thid);
    pthread_once(&once,run);
    printf("thread2 ends\n");
}

int main(int argc, char *argv[]) {
    pthread_t thid1;
    pthread_t thid2;
   
    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(3);
    printf("main thread exit!\n");

    exit(0);
}

