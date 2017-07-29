#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread(void *arg){
	printf("this is a new thread, thid = %lu\n", pthread_self());
	sleep(1);
	return NULL;
}

int main (void)
{
	pthread_t thid;

	printf("main thread, thid = %lu\n", pthread_self());
	if(pthread_create(&thid, NULL, thread, NULL) != 0){
		printf("thread creation failed\n");
		exit(1);
	}
	sleep(1);

    exit(0);
}
