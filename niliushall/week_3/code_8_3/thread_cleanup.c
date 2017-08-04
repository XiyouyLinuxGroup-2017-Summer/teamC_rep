#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int glob;

void err(const char *str, int line)
{
	perror(str);
	printf("line: %d ", line);
	exit(0);
}

static void cleanupHandler(void *arg) {
	int s;
	printf("cleanup: freeing block at %p\n", arg);
	free(arg);

	printf("cleanup: unlocking mutex\n");
	s = pthread_mutex_unlock(&mutex); //在此之前已上锁，而此后没有unlock函数
	if(s != 0)
		err("unlock", __LINE__);

}

static void *threadFunc(void *arg) {
	int s;
	void *buf = NULL;

	buf = malloc(0x10000);
	printf("thread: allocated memory at %p\n", buf);

	pthread_mutex_lock(&mutex); //加锁
	if(s != 0)
		err("mutex_lock", __LINE__);

	pthread_cleanup_push(cleanupHandler, buf);

	while(glob == 0){
		s = pthread_cond_wait(&cond, &mutex); //若被pthread_cancel函数取消线程，则后面的代码不再执行，而是自动调用清理函数
		if(s != 0)
			err("wait", __LINE__);
	}

	printf("thread: condition wait loop completed\n");
	pthread_cleanup_pop(1);

	return NULL;
}

int main (int argc, char **argv)
{
	pthread_t thr;
	void *res; //pthread_join的参数
	int s;

	s = pthread_create(&thr, NULL, threadFunc, NULL); // 新建线程，并执行threadFunc函数
	if(s != 0)
		err("pthread_create", __LINE__);

	sleep(2);

	if(argc == 1) { //无参数
		printf("main: about to cancel thread\n");
		s = pthread_cancel(thr); //取消线程，wait后不再执行，自动调用清理函数(pthread_cleanup_push的参数里的清理函数)
		if(s != 0)
			err("cancel", __LINE__);
	} else { //有参数
		printf("main: about to  signal condition variable\n");
		glob = 1;
		s = pthread_cond_signal(&cond); //唤醒wait
		if(s != 0)
			err("signal", __LINE__);
	}

	s = pthread_join(thr, &res); //等待thr线程结束，并将线程结束返回值存到res
	if(s != 0)
		err("join", __LINE__);

	if(res = PTHREAD_CANCELED) //线程被pthread_cancel函数取消
		printf("main: thread was canceled\n");
	else
		printf("main: thread terminated normally\n");

	exit(EXIT_SUCCESS);

}
