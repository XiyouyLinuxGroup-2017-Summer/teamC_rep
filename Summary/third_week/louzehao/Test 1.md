# 实验1

## Code

``` c
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_CHILD_NUMBER 10
#define SLEEP_INTERVAL 2

int proc_number = 0;

void do_something();

int
main(int argc, char *argv[])
{
  int child_proc_number = MAX_CHILD_NUMBER;
  int i, ch;
  pid_t child_pid;
  pid_t pid[10] = { 0 };

  if (argc > 1)
    {
      child_proc_number = atoi(argv[1]);
      child_proc_number = (child_proc_number > 10) ? 10 : child_proc_number;
    }

  for (i = 0; i < child_proc_number; i++)
    {
      proc_number = i;
      if ((pid[i] = fork()) == 0)
        {
          do_something();
        }
    }

    while ((ch = getchar()) != 'q')
      {
        if (isdigit(ch))
          {
            kill(pid[ch - '0'], SIGTERM);
          }
      }

    kill(0, SIGKILL);
    return 0;
}

void
do_something()
  {
    for (;;)
      {
        printf("This is process No.%d and its pid is %d\n", proc_number, getpid());
        sleep(SLEEP_INTERVAL);
      }
  }
```

## Answer

- proc_number 这个全局变量在各个子进程里的值相同吗？为什么？
  - 不相同，创建时被更改了。
- 使用kill 命令可以在进程的外部杀死进程。进程怎样能主动退出？这两种退出方式哪种更好一些？
  - 通过`abort` `exit`来退出进程。

# 实验二

## Code

``` c
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<pthread.h>

#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD]; 
/* unsigned long  long是比long还长的整数 */

void* thread_worker(void*); 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc,char* argv[])
{
    int i, rtn, ch;
    pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/
    int t[3];
    for (i=0; i<MAX_THREAD; i++)
    {
        t[i] = i;
        pthread_create(&pthread_id[i], NULL, thread_worker, (void *)&t[i]);
        /* 在这里填写代码，用pthread_create建一个普通的线程，
        线程id存入pthread_id[i]，线程执行函数是thread_worker
        并i作为参数传递给线程 */
        //sleep(1);
    }
    do
    {
        /* 用户按一次回车执行下面的循环体一次。按q退出 */
        pthread_mutex_lock(&mutex);
        unsigned long long sum = 0;
        /* 求所有线程的counter的和 */
        for (i=0; i<MAX_THREAD; i++)
        {
            /* 求所有counter的和 */
            sum += counter[i];
            printf("%llu ", counter[i]);
        }

        printf("%llu/%llu\n", main_counter, sum);
        pthread_mutex_unlock(&mutex);
    }while ((ch = getchar()) != 'q');
    pthread_mutex_destroy(&mutex);
    return 0;
}
void* thread_worker(void* p)
{
    int thread_num;
    thread_num = *(int *)p;
    /* 在这里填写代码，把main中的i的值传递给thread_num */
    for(;;)
    { /* 无限循环 */
        pthread_mutex_lock(&mutex);
        counter[thread_num]++; /* 本线程的counter加一 */
        main_counter++; /* 主counter 加一 */
        pthread_mutex_unlock(&mutex);
    }
}
```

## Answer

- thread 的CPU 占用率是多少？为什么会这样？
  - 38%左右，死循环导致CPU被占用。
- 使你最初认为前三列数会相等吗？最后一列斜杠两边的数字是相等，还是大于或者小于关系？
  - 不相同，既可以大于也可以小于，小于的情况更多。

# 实验三

## Code

``` c
//* POSIX 下线程死锁的演示程序 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define LOOP_TIMES 10

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

## Answer

- deadlock实际运行结果如何？
  - 在进行一段时间之后，两个线程均被阻塞