/*************************************************************************
	> File Name: createthread.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 09时43分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int* thread(void *arg) {
    pthread_t newthid;
    
    newthid = pthread_self();
    printf("this is a new thread,thread ID = %u\n",newthid);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thid;
    
    printf("main thread,ID is %u\n",pthread_self());
    if(pthread_create(&thid,NULL,(void *)thread,NULL) != 0) {
        printf("thread creation failed\n");
        exit(1);
    }

    sleep(1);
    exit(0);
}

