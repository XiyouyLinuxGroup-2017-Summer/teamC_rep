#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

static void *threadFunc(void *arg) {
	char *s = (char *)arg;
	printf("%s\n", s);

	return (void *) strlen(s);
}

int main(int argc, char const *argv[])
{
	pthread_t t1;
	void *res;
	int s;

	s = pthread_create(&t1, NULL, threadFunc, "hello world");
	if(s != 0) {
		printf("pthread_create error\n");
		exit(-1);
	}

	printf("message from main()\n");
	s = pthread_join(t1, &res);
	if(s != 0) {
		printf("join error\n");
		exit(1);
	}

	printf("thread returned %ld\n", (long)res);
	exit(EXIT_SUCCESS);

	return 0;
}
