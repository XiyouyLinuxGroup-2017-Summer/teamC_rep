/*************************************************************************
	> File Name: xiancheng1.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月04日 星期五 15时28分46秒
 ************************************************************************/

//* POSIX 下线程死锁的演示程序 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define LOOP_TIMES 10000 

/*用宏PTHREAD_MUTEX_INITIALIZER来初始化 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void*);
void critical_section(int thread_num, int i); 

int main(void) 
{     
    int rtn, i;     
    pthread_t pthread_id = 0; /* 存放子线程的id */ 
    rtn = pthread_create(&pthread_id,NULL, thread_worker, NULL ); 
    if(rtn != 0)
    {            
        printf("pthread_create ERROR!\n"); 
        return -1; 
    } 
    for (i=0; i<LOOP_TIMES; i++) 
    { 
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2); 
        critical_section(1, i); 
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    } 

    pthread_mutex_destroy(&mutex1); 
    pthread_mutex_destroy(&mutex2); 
    return 0;
} 
void* thread_worker(void* p) 
{ 
    int i; 
    for (i=0; i<LOOP_TIMES; i++)
    { 
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2); 
        critical_section(2, i); 
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
    }
} 
void critical_section(int thread_num, int i) 
{ 
    printf("Thread%d: %d\n", thread_num,i);
}
