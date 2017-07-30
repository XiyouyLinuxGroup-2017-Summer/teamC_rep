#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void thread(void *arg){
	printf("i am helping to do something\n");
	sleep(3);
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	pthread_t thid;
	int status;

	pthread_create(&thid, NULL, (void *)thread, NULL);
	pthread_join(thid, (void *)&status);
	printf("thread exit is caused %d\n", status);

	return 0;
}