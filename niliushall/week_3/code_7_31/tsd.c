#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *thread1(void *);
void *thread2(void *);

pthread_key_t key;

void *thread1(void *arg){
	int a = 1, *tsd = &a;
	pthread_t thid2;

	printf("thread1 %lu is running\n", pthread_self());
	pthread_setspecific(key, tsd);
	pthread_create(&thid2, NULL, thread2, NULL);
	sleep(3);  //---2
	tsd = (int *)pthread_getspecific(key);
	printf("thread1 %lu returns %d\n", pthread_self(), *tsd);

	return (void *)0;
}

void *thread2(void *arg){
	int a = 5, *tsd = &a;
	printf("thread2 %lu is running\n", pthread_self());
	pthread_setspecific(key, (int *)tsd);
	tsd = (int *)pthread_getspecific(key);
	printf("thread2 %lu returns %d\n", pthread_self(), *tsd);//tsd is a pointer

	return (void *)0;
}

int main(int argc, char const *argv[])
{
	pthread_t thid1;

	printf("main thread is running\n");
	pthread_key_create(&key, NULL);
	pthread_create(&thid1, NULL, thread1, NULL);
	sleep(5);  //---1
	pthread_key_delete(key);
	printf("main thread exit\n");
	
	return 0;
}
