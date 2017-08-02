#include <stdio.h>
#include <pthread.h>
#include <string.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

#define MAX_ERROR_LEN 256

static void destructor(char *buf) {
	free(buf);
}

static void createKey(void) {
	int s;
	s = pthread_key_create(&strerrorKey, destructor);
	if(s != 0){
		printf("error1\n");
		exit(1);
	}
}

char *strerror(itn err) {
	int s;
	char *buf;

	s = pthread_once(&once, createKey);
	if(s != 0){
		printf("error2\n");
		exit(0);
	}

	s = pthread_getspecific(strerrorKey);
	if( s == NULL) {
		buf = malloc(MAX_ERROR_LEN);
		if(buf == NULL){
			printf("error\n");
			exit(0);
		}
	}

	s = pthread_setspecific(strerrorKey, buf);
	if(s != 0){
		printf("error\n");
		exit(0);
	}

	if(err<0 || err>=_sys_nerr || _sys_errlist[err] == NULL) {
		snprintf(buf, 	_sys_errlist[err], MAX_ERROR_LEN, err);
	} else {
		strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
		buf[MAX_ERROR_LEN - 1] = 0;
	}

	return buf;
}