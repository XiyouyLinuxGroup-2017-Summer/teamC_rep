## 实验1 ##

```
/*  POSIX 下进程控制的实验程序残缺版 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <signal.h>
#include <ctype.h> 
#include <stdlib.h>
/* 允许建立的子进程个数最大值 */
#define MAX_CHILD_NUMBER 10 
/* 子进程睡眠时间 */
#define SLEEP_INTERVAL 2 
int proc_number=0; 
/* 子进程的自编号，从0开始 */
void do_something(); 

int main(int argc, char* argv[])
{   
    /* 子进程个数 */
    int child_proc_number = MAX_CHILD_NUMBER; 
    int i, ch; 
    pid_t  child_pid; 
    pid_t pid[10]={0}; /* 存放每个子进程的id */ 
    if (argc > 1) /* 命令行参数第一个参数表示子进程个数*/ 
    {
        child_proc_number = atoi(argv[1]); 
        child_proc_number= (child_proc_number > 10) ? 10 :child_proc_number;
    } 
    for (i=0; i<child_proc_number; i++) { 
        /* 填写代码，建立child_proc_number个子进程要执行
        * proc_number = i; 
        * do_something(); 
        * 父进程把子进程的id保存到pid[i] */
        child_pid = fork();  
        if (child_pid == -1) {
            printf("failed\n");
        }
        else if(child_pid == 0) {   
            proc_number = i;  
            do_something();  
        } else {  
            pid[i] = child_pid;  
        }     
    }
    /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
    while ((ch = getchar()) != 'q')  
    { 
        if (isdigit(ch)) 
        { 
            /*  填写代码，向pid[ch-'0']发信号SIGTERM， 
            * 杀死该子进程 */ 
            kill(pid[ch-'0'],SIGTERM);
        }
    } 
    /* 在这里填写代码，杀死本组的所有进程 */
    for(i = 0;i < child_proc_number;i++)  
       kill(pid[i],SIGTERM);
    //kill(0,SIGTERM);
    //注意wait,防止僵尸进程
    return;
} 
void do_something() 
{ 
    for(;;) 
    {  
        printf("This is process No.%d and its pid is %d\n",proc_number,  getpid());
        sleep(SLEEP_INTERVAL); // 主动阻塞两秒钟
    }
}
```

 
> 使用kill 命令可以在进程的外部杀死进程。进程怎样能主动退出？这两种退出方式哪种更好一些？
进程只要执行结束所有代码，就可以主动退出，这里是杀了所有进程，才可以退出，如果异常退出，使用kill子进程都变成僵尸进程，因为父进程还没有回收，就已经杀死了。主动退出更好一些。

## 实验2 ##

```
/*  POSIX 下线程控制的实验程序残缺版 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD]; 
/* unsigned long  long是比long还长的整数 */
void* thread_worker(void*); 
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc,char* argv[])
{     
    int i, rtn, ch;     
    pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/ 
    for (i=0; i<MAX_THREAD; i++)
    {       
        /* 在这里填写代码，用pthread_create建一个普通的线程，
        线程id存入pthread_id[i]，线程执行函数是thread_worker
        并i作为参数传递给线程 */
        rtn = pthread_create(&pthread_id[i],NULL,(void *)thread_worker,i);
    }    
    do
    {
        /* 用户按一次回车执行下面的循环体一次。按q退出 */          
        unsigned long long sum = 0;    
        /* 求所有线程的counter的和 */
        pthread_mutex_lock(&mutex);
        for (i=0; i<MAX_THREAD; i++) 
        {
            /* 求所有counter的和 */         
            sum += counter[i];             
            printf("%llu ", counter[i]);        
        }
        printf("%llu/%llu", main_counter, sum); 
        pthread_mutex_unlock(&mutex1);  
    }while ((ch = getchar()) != 'q'); 
    return 0;
} 
void* thread_worker(void* p)
{   
    int thread_num;

    thread_num = (int)p;
    /* 这里填写代码，把main中的i的值传递给thread_num */ 
    for(;;) 
    { /* 无限循环 */      
        pthread_mutex_lock(&mutex1);
        counter[thread_num]++; /* 本线程的counter加一 */ 
        main_counter++; /* 主counter 加一 */   
        pthread_mutex_unlock(&mutex1);
    } 
}

```


> 最后的结果有什么特点？对原因进行分析
>  前三个数字，有两个都会是0，斜杠前面的数字还比后面的小 
因为线程运行的时候是取出全局变量，但是很多线程取一个全局变量，都会把自己改变的全局变量放回原来的地方，无形之中全局变量就少加了很多


> thread 的CPU 占用率是多少？为什么会这样？
cpu占有率能达到300%多，因为线程可以多个处理器跑，所有使用也就比较多 
> 
> thread_worker()内是死循环，它是怎么退出的？你认为这样退出好吗？
 进程结束之后，所以线程也就完美结束，这样不好，因为没有处理临界资源


## 实验3 ##

```
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
```

> 上锁的顺序错误，造成死锁，换一下就可以了


