/*************************************************************************
	> File Name: myerrno.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 17时59分22秒
 ************************************************************************/

#include <stdio.h>   
#include <string.h>   
#include <pthread.h>   
    
int my_errno = 0;   
pthread_key_t key;   
    
void print_errno(char *str)   
{   
    printf("%s my_errno:%d\n",str, my_errno);   
}   
void *thread2(void *arg)   
{   
    printf("thread2 %ld is running\n",pthread_self());   
    pthread_setspecific(key, (void *)my_errno);   
    printf("thread2 %ld returns %d\n",pthread_self(),\   
            pthread_getspecific(key));   
    my_errno = 2;       
    print_errno("thread2");   
}   
    
void *thread1(void *arg)   
{   
    pthread_t thid2;   
    
    printf("thread1 %ld is running\n",pthread_self());   
    pthread_setspecific(key, (void *)my_errno);   
    pthread_create(&thid2, NULL, thread2, NULL);   
    sleep(2);   
    printf("thread1 %ld returns %d\n",pthread_self(),\   
         pthread_getspecific(key));   
    my_errno = 1;   
    print_errno("thread1");   
}   
    
void destr(void *arg)   
{   
    printf("destroy memory\n");   
}   
    
int main(void)   
{   
    pthread_t thid1;   
    
    printf("main thread begins running. my_errno=%d\n",my_errno);   
    pthread_key_create(&key, destr);   
    pthread_create(&thid1, NULL, thread1, NULL);   
    sleep(4);   
    pthread_key_delete(key);   
       
    printf("main thread exit\n");   
    
    return 0;   
}   
