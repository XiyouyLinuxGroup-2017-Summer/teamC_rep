#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int err(const char *str, int line) {
	perror(str);
	printf("line %d: ", line);
	exit(0);
}

static void * threadFunc(void *arg) {
	int i;
	printf("New thread started\n");
	for(i = 1; ; i++) {
		printf("loop %d\n", i);
		sleep(1);
	}

	return NULL;
}
int main (void) {
	pthread_t thr;
	int s;
	void *res;

	s = pthread_create(&thr, NULL, threadFunc, NULL);
	if(s != 0)
		err("pthread_create", __LINE__);
	
	//printf("\n1111\n");
	sleep(3);

	//printf("\n2222\n");
	s = pthread_cancel(thr);
	if(s != 0)
		err("pthread_cancel", __LINE__);

	s = pthread_join(thr, &res);
	if(s != 0)
		err("pthread_join", __LINE__);

	if(res == PTHREAD_CANCELED)
		printf("THread was canceled\n");
	else
		printf("THread was not canceled!\n");

	exit(EXIT_SUCCESS);
}