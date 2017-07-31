#include <pthread.h>

pthread_mutex_t number_mutex;
int globalnumber;

int write_globalnumber(void) {
	pthread_mutex_lock(number_mutex);
	globalnumber++;
	pthread_mutex_unlock(number_mutex);
}

int read_globalnumber(void) {
	int temp;
	pthread_mutex_lock(number_mutex);
	temp = globalnumber;
	pthread_mutex_unlock(number_mutex);

	return (temp);
}