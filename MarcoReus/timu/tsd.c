/*************************************************************************
	> File Name: tsd.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 17时31分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_key_t key;
void *thread1(void *arg);
void *thread2(void *arg);


void *thread1(void *arg) {
    int tsd = 0;
    pthread_t thid2;

    printf("thread %d is running\n",pthread_self());
    pthread_setspecific(key,(void *)tsd);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(5);
    printf("thread %d returns %d\n",pthread_self(),pthread_getspecific(key));
}

void *thread2(void *arg) {
    int tsd = 5;
    printf("thread %d is running\n",pthread_self());
    pthread_setspecific(key,(void *)tsd);
    printf("thread2 %d returns %d\n",pthread_self(),pthread_getspecific(key));
}


int main(int argc, char *argv[]) {
    pthread_t thid1;
    printf("main thread begins running\n");
    pthread_key_create(&key,NULL);
    pthread_create(&thid1,NULL,thread1,NULL);
    sleep(3);
    pthread_key_delete(key);
    printf("main thread exit\n");

    return 0;
}

