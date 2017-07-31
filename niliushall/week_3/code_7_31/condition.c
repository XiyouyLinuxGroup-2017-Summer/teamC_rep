#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void *thread1(void *arg) {
	pthread_cleanup_push((void(*)(void *))pthread_mutex_unlock, &mutex);

	while(1) {
		printf("thread1 is running\n");
		//pthread_mutex_lock(&mutex);
		//pthread_cond_wait(&cond, &mutex);
		printf("thread1 applied the conditon\n");
		//pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	pthread_cleanup_pop(0);
}

void *thread2(void *arg) {
	while(1) {
		printf("thread2 is running\n");
		//pthread_mutex_lock(&mutex);
		//pthread_cond_wait(&cond, &mutex);
		printf("thread2 applied the condition\n");
		//pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	pthread_t tid1, tid2;
	printf("condition variable study\n");
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid1, NULL, thread1, NULL);
	pthread_create(&tid2, NULL, thread2, NULL);

	do {
		pthread_cond_signal(&cond);
	}while(1);

	sleep(50);
	pthread_exit(0);

	return 0;
}
