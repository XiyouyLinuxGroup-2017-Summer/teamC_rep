#include<pthread.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
pthread_key_t key;
int init_done = 0;
pthread_barrier_t bar;

void destructor(void *arg) {}

void *threadfunc1(void *arg) {
	if (!init_done) {
		init_done = 1;
		int err = pthread_key_create(&key, destructor);
		if (err != 0)
			printf("err!%s\n", strerror(err));
	}
	printf("[THREAD1]%u\n", (unsigned int)key);
	pthread_barrier_wait(&bar);
}

void *threadfunc2(void *arg) {
	printf("[THREAD2]%u\n", (unsigned int)key);
	pthread_barrier_wait(&bar);
}

int main() {
	pthread_t t1, t2;
	pthread_barrier_init(&bar, NULL, 3);
	pthread_create(&t2, NULL, threadfunc2, NULL);
	pthread_create(&t1, NULL, threadfunc1, NULL);
	
	printf("[MAIN]%u\n", (unsigned int)key);
	pthread_barrier_wait(&bar);

	pthread_barrier_destroy(&bar);
}
