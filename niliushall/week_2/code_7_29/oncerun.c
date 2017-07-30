#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
pthread_once_t once = PTHREAD_ONCE_INIT;

void run (void){
	printf("function is running in thread %lu\n", pthread_self());
}

void * thread1(void *arg){
	printf("current thread1 ID is %lu\n", pthread_self());
	pthread_once(&once, run);
	printf("thread1 ends\n");
}

void *thread2(void *arg){
	printf("current thread2 ID is %lu\n", pthread_self());
	pthread_once(&once, run);
	printf("thread2 ends\n");
}

int main(int argc, char const *argv[])
{
	pthread_t thid1, thid2;

	pthread_create(&thid1, NULL, thread1, NULL);
	pthread_create(&thid2, NULL, thread2, NULL);
	sleep(3);
	printf("main thread ends\n");

	return 0;
}