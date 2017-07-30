# Summary

这一周实际上没有怎么写自己的代码，每天做一下Virtual Judge上面的题目算是其中比较大的代码量，大概还有对`pshell`根据需求进行了一点点小的修改。最后开了一点网络编程的小坑，`libportal`有了一点点小的进展。
这一周更多的还是在看书学习，主要是学习线程相关的内容，线程并没有在Linux内核实现，而是通过函数库，使用一种轻量级的进程的方法实现。线程的内容更多的侧重于同步、私有数据方面。最后开了一点点网络编程的头。总体来说这一周还是比较满意的。
在这里顺便理以下自己写`pshell`的一些问题:
1. 父进程中应当使用`waitpid`来等待子进程结束，原因是若去使用`wait`，当之前有一个后台运行的进程已经结束时，会获取到那个进程的状态，导致之后的提示符错位。
2. 构建`argv[]`时，最后一个元素应该是`(char *)NULL`，否则会出现`Bad Address`的错误。
3. 其实使用带p的`exec`族函数可以自动在`path`环境变量中查找，不需要手动进行。

对于其他方面来说，每天过得感觉很快，尚待自己学习的内容也很多，可能自己的状态这一周不是很好，下一周一定要好好休息，每天花费那么多的时间去调整状态还是过于奢侈。

> 道阻且长，加油！

# 以下是技术性的学习总结

# 进程标识符

### 函数原型及解释

``` c
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
gid_t getegid(void);
```

 - 返回值：成功时`getpid`返回自身的`pid`，`getppid`返回自身父进程的`pid`，`getuid`返回自身的实际运行`uid`，`geteuid`返回自身的有效运行`uid`，`getgid`返回自身实际运行`gid`，`getegid`返回自身有效运行`gid`。这些函数**均没有出错返回**。

# 创建子进程

### 函数原型及解释

``` c
pid_t fork(void);
pid_t vfork(void);
```

- 返回值：若函数执行失败，则返回`-1`，若函数执行成功，由于产生了子进程，因此函数将有两个返回值。在父进程中，函数将返回`子进程的pid`，在子进程中，函数将返回`0`。

### 其他相关解释

 - 返回值为何如此设计：一个父进程可以有多个子进程，因此`fork`需要告诉父进程新的子进程的`pid`的值是多少，然而一个子进程只有一个父进程，因此子进程可以轻易的使用`getppid`函数获得到父进程的`pid`。
 - 执行完函数之后会发生什么：若执行成功，从父进程和子进程会共同执行从`fork`函数开始的接下来的所有代码，但是使用`fork`函数创建子进程的话父进程和子进程谁先开始执行是不确定的。
 - `fork`和`vfork`函数的区别：`vfork`函数创建的子进程的目的应该是执行`exec`族函数。除此之外，`vfork`函数保证了子进程在创建成功之后一定优先运行，且在子进程执行`exec`族函数之前或子进程退出之前（两者满足其一即可）父进程都不会被内核调度运行。
 - `fork`族函数中父进程与子进程的关系：①调用`fork`族函数时，父进程打开的**所有**文件描述符将被复制到子进程中；②父子进程将共享**同一个**文件偏移量；③子进程将从父进程中继承`实际uid`、`实际gid`、`有效uid`、`有效gid`、`附属组id`、`进程组id`、`会话id`、`控制终端`、`当前工作目录`、`umask值`、`环境变量`、`资源限制`等等一系列属性。
 - `fork`族函数运行失败的原因：一般有以下两种情况：①系统中已经有了太多的进程；②该实际用户ID下的进程数量超过了系统的限制。

# 进程的终止

### 正常终止

进程的正常终止方式有以下六种：

> ①`main`函数中调用了`return`。
> ②在进程的任意位置调用了`exit`函数。

以上两种情况是同义的，他们都将调用终止处理程序，即使用了`atexit`函数登记的函数，并关闭所有标准I/O流。

> ③ 在进程的任意位置调用`_exit`函数。
> ④ 在进程的任意位置调用`_Exit`函数。

以上两个情况也是同义的，他们都**不会**调用终止处理程序，而且不会冲洗I/O流。

> ⑤最后一个线程在启动例程中执行了`return`语句。
> ⑥最后一个线程调用了`pthread_exit`函数。

以上两个情况中，终止的进程返回值一定为`0`，与线程的返回值没有任何关系。

### 异常终止

进程的异常终止有以下三种情况

> ①进程的任意位置调用了`abort`函数（这将产生`SIGABRT`信号）。
> ②进程收到一些信号时。
> ③最后一个线程对`取消`请求做出相应。
 
可以看到前两种情况中，第一种情况是第二种情况的一个特例。

### 其他相关

 - 若子进程在运行时父进程已经结束，此时子进程会成为“孤儿进程”，内核会将之置为`init`进程的子进程。

# 等待子进程结束

### 函数原型及解释

``` c
pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);
```

 - 返回值：若函数执行成功，则返回`得到状态的进程id`；若函数执行出错则在大部分情况下返回`-1`，在指定了一定参数时返回`0`。
 - `stat_loc`：指定为一个`int`指针。函数将把获取到的进程终止时的状态存储在该指针指向的区域。若将参数设置为`NULL`，则表示父进程不关心子进程的终止状态，将会丢弃这部分内容。
 - `pid`：指定要等待其结束的进程的`pid`，函数将仅在它结束的时候返回。若指定为`-1`，此种情况下`waitpid`与`wait`等效；若指定为`0`，此时将等待`gid`等同于调用`进程组id`的任一子进程；其他情况下等同于等待`pid`等于该参数绝对值的进程。
 - `options`：`waitpid`函数的选项设定，可以参看下面参数一节。
 
### 其他相关解释

 - 两种函数的区别：①`wait`会使调用者阻塞，`waitpid`可以通过`options`参数实现不阻塞；②`waitpid`可以指定等待哪一个具体的进程结束。
 - `zombie`（僵死进程）：当子进程终止时，不会完全地清除掉其所有状态，而是保存了至少`pid`、`终止状态`、`使用的CPU时间总量`等状态的信息。父进程可以通过`wait`或者`waitpid`来获取这些属性。若父进程没有处理这些信息，此时的子进程就称之为`zombie`（僵死进程）。
 - 信号系统相关：子进程终止时内核会向父进程发送`SIGCHLD`信号，父进程既可以以此来设计控制系统处理该信号，也可以选择忽略该信号（但子进程的终止状态会一直保存）。

### 参数

其中，`options`可以设置为以下四种情况相互做位或运算得到的值。

|选项|意义|
|-|-|
|`0`|不设置任何特殊功能|
|`WNOHANG`|若`pid`指定的子进程不是立即可用的，则`waitpid`不会阻塞，而是直接返回`0`|
|`WCONTINUED`|若实现支持作业控制，则由`pid`指定的任一子进程在停止后已经继续，但其状态尚未报告，则返回其状态。|
|`WUNTRACED`|若实现支持作业控制，而由`pid`指定的任一子进程已处于停止状态，并且其状态自停止以来还未报告过，则返回其状态。|

其中，通过`stat_loc`参数拿到的值可以使用以下宏来检测其终止状态：

|宏|意义|
|-|-|
|`WIFEXITED(stat_loc)`|若为正常终止的子进程返回的状态，则为真|
|`WEXITSTATUS(stat_loc)`|获得正常终止的子进程传给`exit`等函数参数的低8位|
|`WIFSIGNALED(stat_loc)`|若为异常终止的子进程返回的状态，则为真|
|`WTERMSIG(stat_loc)`|获得使子进程异常终止的信号编号|
|`WIFSTOPPED(stat_loc)`|若为当前暂停的子进程返回的状态，则为真|
|`WSTOPSIG(stat_loc)`|获得使子进程暂停的信号编号|
|`WIFCONTINUED(stat_loc)`|若在作业控制暂停后已经继续的子进程返回的状态，则为真|
|`WCOREDUMP(stat_loc)`|若产生了终止进程的`core`文件，则为真|

# 执行另一个程序

### 函数原型及解释

``` c
int execl(const char *path, const char *arg, ... /* (char  *) NULL */);
int execlp(const char *file, const char *arg, ... /* (char  *) NULL */);
int execle(const char *path, const char *arg, ... /*, (char *)NULL, char * const envp[] */);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
```

 - 返回值：若函数执行成功无返回值，函数执行失败则返回`-1`。
 - `path`：要执行的程序的路径。既可以是绝对路径也可以是相对路径。
 - `file`：要执行的程序的文件名。若指定的参数中包含“/”，则将他认为是一个路径，若不包含，则将在`PATH`环境变量中指定的目录下进行搜索。
 - `arg ...`：在此处罗列所有传递给程序的命令行参数，最后一个参数之后要附加一个`(char *) NULL`来表示结束。
 - `argv`：提供一个命令行参数的数组，数组的最后一个字符串一定为`NULL`。
 - `envp`：提供一个环境变量数组。
 - `fd`：要执行的文件的文件描述符。

### 其他相关解释

 - 为何成功执行无返回值：因为`exec`族函数会将新的程序的代码段完全地代替子进程的代码，因此若成功执行了`exec`族函数，返回值已经没有意义，所以没有返回值。
 - 执行完`exec`族函数之后，子进程的`pid`并没有发生变化，从父进程继承的各种属性也依然有效。
 - 在以上函数中，但凡不需要指定`envp[]`参数的，都会将`environ`变量传入，当做环境变量来使用。
 
### 区分`exec`族函数

![exec族函数的区分](http://img.blog.csdn.net/20170724151807990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 - `exec`后第一个字母 
   - 凡是使用参数表的，均带有`l`，代表`list`。
   - 凡是使用argv数组的，均带有`v`，代表`vector`。
 - `exec`后第二个字母
   - 凡是使用路径的，此位均留空。
   - 凡是使用文件名的，此为均为`p`。
 - `exec`后第三个字母
   - 凡是使用`environ`变量的，此位留空。
   - 凡是使用`envp[]`数组的，此位均为`e`。
 - 使用文件描述符的仅有一个，就是`fexecve`。

# 进程运行标识

### 函数原型及解释

``` c
int setuid(uid_t uid);		//设置进程实际用户ID
int setgid(gid_t gid);		//设置进程实际组ID
int seteuid(uid_t uid);		//设置进程有效用户ID
int setegid(gid_t gid);		//设置进程有效用户组ID
```

 - 返回值：函数若执行成功，则返回`0`，若失败则返回`-1`。
 - `uid`：要设置的`uid`。
 - `gid`：要设置的`gid`。

# 进程调度

### 函数原型及解释

```c
int nice(int incr);

int getpriority(int which, id_t who);
int setpriority(int which, id_t who, int value);
```

 - 返回值：`nice`和`getpriority`若执行成功则返回新的`nice`值，若执行出错则返回`-1`；`setpriority`若执行成功则返回`0`，出错则返回`-1`。
 - `incr`：要增加的`nice`值。若`incr`取负数，则会减少`nice`值。当`incr`参数的取值超过指定范围，会自动将之设置为可以取到的最大值。
 - `which`：表示`who`属于那种类型。
 -  `who`：指定要修改进程、进程组或用户的`ID`。  若指定为`0`，会根据参数解释为当前的进程ID、进程组ID或实际用户ID。
 -  `value`：要改变多少。
 
### 参数

其中，`which`参数可取以下值：

|选项|意义|
|-|-|
|`PRIO_PROCESS`|表示`who`是一个`pid`|
|`PRIO_PGRP`|表示`who`是一个`gid`|
|`PRIO_USER`|表示`who`是一个`uid` |

# 会话

### 函数原型及解释

``` c
pid_t setsid(void);
pid_t getsid(pid_t pid);
```

 - 返回值：若成功，则返回`进程组id`，若执行失败则返回`-1`。
 - `pid`：若指定为`0`，则使之返回调用进程的会话首进程的进程组ID。

### 其他相关解释

 - 调用`setsid`函数的进程若不是一个进程组的组长，此函数将创建一个新的会话，并：①将该进程变成新会话的首进程；②该进程成为一个新进程组的组长进程，进程组的ID为调用该函数进程的`pid`；③该进程没有控制终端，若在调用`setsid`之前该进程有一个控制终端，那么这种联系也被切断。
 - 可以使用这个程序来创建一个守护进程。

 > **线程包含的信息**：每个线程中都包含有表示自身执行环境的信息，其中至少包括：标识线程的`线程ID`、`一组寄存器的值`、`栈`、`调度优先级和策略`、`信号屏蔽字`、`errno变量`和`线程的私有数据`。
> **线程函数的错误处理与返回值**：与一般的POSIX函数不同，线程函数在执行发生错误的时候，一般不会设置`errno`全局变量值，而是直接返回表示错误类别的错误码。因此下面的函数若不特殊说明，一般不会设置`errno`。

# 线程标识符

### 函数原型及解释

``` c
int pthread_equal(pthread_t t1, pthread_t t2);	//判断两个线程ID是否相同
pthread_t pthread_self(void);					//获得自身的线程ID
```

 - 返回值：`pthread_equal`函数两个参数表示的线程ID若相同，则返回`非0值`，反之则返回`0`；而`pthread_self`函数将返回调用线程的线程ID。
 - `t1`、`t2`：要判断的两个线程ID。

# 创建线程

### 函数原型及解释

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
```
 - 返回值：若创建成功，则返回`0`，若创建失败，则返回错误号。
 - `thread`：若创建成功，`thread`参数指向的区域将被设置为新线程的线程ID。
 - `attr`：改变这个参数可以定制线程的属性。这个值可以设为`NULL`，表示按照默认的配置创建新线程，其他值的选取可以参考下面“参数”一节。
 - `start_routine`：新线程的起始函数，该函数指针应该仅仅接受一个`void *`类型的参数。
 - `arg`：传给新线程起始函数的参数。这里应该是一个`void *`类型的参数。

### 其他相关解释

 - 新线程创建之后，并不能保证哪一个线程一定先开始运行。
 - 新线程可以访问进程的地址空间，并继承调用线程的浮点环境和浮点屏蔽字，但挂起信号集会被清除。

### 参数

其中`attr`参数是一个`pthread_attr_t`类型的参数，对他进行操作需要用到以下函数，以下函数调用成功均返回`0`，失败则返回相应的`错误码`：

``` c
//初始化一个pthread_attr类型
int pthread_attr_init(pthread_attr_t *attr);
//销毁一个pthread_attr类型
int pthread_attr_destroy(pthread_attr_t *attr);

//设置/获取一个线程的分离状态，其中detachstate可以设置为：
//①PTHREAD_CREATE_DETACHED 分离状态
//②PTHREAD_CREATE_JOINABLE 正常状态
int pthread_attr_setdetachstate(pthread_attr_t *attr,int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

//设置栈的开始地址
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);

//设置/获取线程的栈大小
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

//设置/获取栈警戒缓冲区的大小
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
```


# 线程终止

### 线程何时终止？

当出现以下三种情况时，线程将终止：
1. 线程从启动函数中返回，此时返回值将作为线程的退出码。
2. 线程被同一进程中的其他线程取消。
3. 线程在自身函数调用中使用了`pthread_exit`函数。

### 函数原型及解释

``` c
void pthread_exit(void *retval);
```
 - `retval`：该参数将作为线程的返回值，我们可以使用`pthread_join`来获得到这个指针。

# 等待线程终止并获取其终止状态

### 函数原型及解释

``` c
int pthread_join(pthread_t thread, void **retval);
```

 - 返回值：若函数执行成功，则返回`0`，发生错误则会返回`错误码`。
 - `thread`：要获取终止状态的线程ID。
 - `retval`：线程的终止状态将写入`retval`指向的内存区域。
 
### 其他相关解释

 - 在执行`pthread_join`函数时，调用线程将被一直阻塞直到指定的线程终止为止。若指定线程是以`在启动函数中return`方式结束，则`retval`为`return`的返回值；若指定线程是以调用`pthread_exit`函数的方式结束，则`retval`为函数的`retval`参数；若指定线程是被其他线程取消的，则`retval`将被指定为`PTHREAD_CANCELED`。
 - 若调用线程对终止进程的返回值并不在意，可以将`retval`设置为`NULL`，这样函数将只等待指定线程结束，而不获取其终止状态。

### 分离状态

默认情况下，线程的终止状态将会保存，直到对该线程调用`pthread_join`函数为止。但是若线程处于分离状态，那么线程终止之后其占用的所有空间将会立即被收回。对一个分离状态结束的进程使用`pthread_join`是一个**未定义行为**。
 
# 取消线程

### 函数原型及解释

```c
int pthread_cancel(pthread_t thread);
```
 - 返回值：若函数执行成功，则返回`0`，发生错误则会返回`错误码`。
 - `thread`：要取消的线程的线程ID。
 
### 其他相关解释

 - `pthread_cancel`函数不会等待指定的线程结束，根据内核的调度策略的不同，即使函数执行成功，该线程也不一定已经结束。

# 线程的退出处理

### 函数原型及解释

``` c
void pthread_cleanup_push(void (*routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);
```

 - `routine`：将该函数指针指向的函数设定为调用线程的一个退出处理函数。
 - `arg`：在执行退出处理时，向`routine`参数指定的函数指针传递这个参数。
 - `execute`：若`execute`不为`0`，将执行由`pthread_cleanup_push`登记的所有函数。

### 其他相关解释

 - 由`pthread_cleanup_push`函数指定的线程退出函数的调用顺序符合栈的顺序。
 - 当调用`pthread_exit`函数、线程相应取消请求、以非零参数执行`pthread_cleanup_pop`时，才将执行登记的退出函数。当线程以从启动函数中返回的方式结束时，不会执行这些函数。
 - 若将`execute`参数的值指定为`0`，虽然不执行退出函数，但仍然将清除所有登记过的退出函数。

# 进程与线程原语对比

![进程与线程原语对比](http://img.blog.csdn.net/20170725173215299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 以下内容中读写锁、自旋锁、屏障并没有在*《Linux C编程实战》*一书中提及，其内容来源于*《Advanced Programming in the Unix Environment》*中。

# 互斥量 Mutex

## 初始化、销毁

### 函数原型及解释

``` c
//对于静态分配的互斥量，可以通过赋特殊值的方法完成初始化，如：
pthread_mutex_t one = PTHREAD_MUTEX_INITIALIZER;
//对于动态分配的互斥量，必须以函数的形式初始化
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const thread_mutexattr_t *restrict attr);

//对于动态分配的互斥量，在释放他的空间之前必须调用销毁函数
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

 - 返回值：若函数执行成功，则返回`0`；函数若执行出错，则返回表示错误类型的`错误码`。
 - `mutex`：表示要初始化、销毁哪一个互斥量。
 - `attr`：表示初始化互斥量的属性，如果要以默认形式初始化一个互斥量，可以将这个参数指定为`NULL`。也可以设定为在下面“参数”一节中指定的任何参数。

### 参数

其中`attr`参数是一个`pthread_mutexattr_t`类型的参数，对他进行操作需要用到以下函数，以下函数调用成功均返回`0`，失败则返回相应的`错误码`：

``` c
//初始化一个pthread_mutexattr_t类型
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
//销毁一个pthread_mutexattr_t类型
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);

//获得和设置互斥量的“进程共享”属性
//若将之设为PTHREAD_PROCESS_PRIVATE，将不提供共享属性
//若将之设为PTHREAD_PROCESS_SHARED，将把他设置为共享的
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr, int *restrict pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);

//获得和设置互斥量的“健壮”属性，健壮属性可以使一个锁被已经终止而未解锁的应用程序获得时，返回一个特殊的值EOWNERDEAD，表示该互斥量需要恢复。
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr, int *restrict robust);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);

//获得和设置互斥量的锁定特性，其中type属性可以为以下几种值
//PTHREAD_MUTEX_NORMAL 若未解锁时就重新加锁，会导致死锁；在不占用和已经解锁时使用解锁的行为未定义
//PTHREAD_MUTEX_ERRORCHECK 当在未接锁时重新加锁、不占用和已经解锁时使用解锁都会返回一个错误码
//PTHREAD_MUTEX_RECURSIVE 允许在未解锁时重新加锁，但是在不占用和已经解锁时使用解锁都会返回一个错误码
//PTHREAD_MUTEX_DEFAULT 当在未接锁时重新加锁、不占用和已经解锁时使用解锁的行为均未定义
int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr, int *restrict type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```

## 加锁、解锁

### 函数原型及解释

``` c
//尝试加锁，若互斥量不能加锁，则阻塞调用线程。
int pthread_mutex_lock(pthread_mutex_t *mutex);
//尝试加锁，若互斥量不能加锁，则直接返回错误码。若可以加锁，将锁住互斥量。
int pthread_mutex_trylock(pthread_mutex_t *mutex);
//解锁
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
 - 返回值：函数若执行成功则返回`0`，否则返回代表错误类型的`错误码`。
 - `mutex`：要尝试加锁、解锁的互斥量。

### 死锁

若一个线程对一个被自己锁住的互斥量再次尝试加锁，则因为他不能加锁将被阻塞，因为其阻塞将不能解锁互斥量，从而导致所有试图访问互斥量的线程全部阻塞。

### 避免死锁：最长等待时间版本的加锁

``` c
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
```

 - 返回值：若执行成功则返回`0`，执行失败则返回代表错误类型的`错误码`。
 - `mutex`：要加锁的互斥量。
 - `abstime`：在加锁之前最长可以等待的时间，这里是一个绝对时间。若不能加锁，线程会至多阻塞由本参数指定的时间。

这里`abstime`是一个`timespec`类型的结构体，它的定义是：
```
struct timespec {
	time_t tv_sec; //表示秒
    long tv_nsec;  //表示纳秒
}
```

所谓的**绝对时间**，是指如果你需要等待3分钟，需要获得到当前的时间，并加上三分钟，而不是单纯的将3分钟转化为一个`timespec`类型。

# 读写锁 Rwlock

## 介绍

读写锁实质上是一种特殊的**自旋锁**，一般而言有三种状态：读模式加锁状态、写模式加锁状态和无锁状态。读写锁一次仅能有一个线程占用写模式的读写锁，但可以有多个线程占用读模式的读写锁。读写锁比较适用于读次数远远大于写次数的数据结构。

当处于读模式加锁状态时，所有的线程都可以给读写锁上锁；但处于写模式加锁状态时，其他任何试图加读模式锁或写模式锁的行为都将被阻塞到写模式锁被释放为止。一般而言，为了防止写模式锁一直被阻塞，当有一个试图加写模式锁而被阻塞的线程时，此时再试图加读模式锁的线程也将被阻塞。

## 初始化、销毁

读写锁在使用之前**必须**被初始化，在使用后，无论是静态分配，还是动态分配都**必须**要被销毁。

### 函数原型及解释

``` c
//静态分配的读写锁可以使用以下两种初始化方式：
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
//动态分配的读写锁只可以使用以下这一种初始化方式：
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);

//销毁读写锁，即使是静态分配的读写锁，使用完毕也应当销毁
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
```

 - 返回值：函数若执行成功，则返回`0`，反之则返回表示错误类型的`错误码`。
 - `rwlock`：要初始化、销毁的读写锁。
 - `attr`：新的读写锁的属性。若设置为`NULL`，则以默认形式初始化一个读写锁，其他的可选参数可以参看下面“参数”一节。

### 参数

其中`attr`参数是一个`pthread_rwlockattr_t`类型的参数，对他进行操作需要用到以下函数，以下函数调用成功均返回`0`，失败则返回相应的`错误码`：

``` c
//初始化一个pthread_rwlockattr_t类型
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
//销毁一个pthread_rwlockattr_t类型
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);

//获得和设置读写锁的“进程共享”属性
//若将之设为PTHREAD_PROCESS_PRIVATE，将不提供共享属性
//若将之设为PTHREAD_PROCESS_SHARED，将把他设置为共享的
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr, int *restrict pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
```

## 加锁、解锁

### 函数原型及解释

``` c
//给一个读写锁加读锁
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
//给一个读写锁加写锁
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
//给一个读写锁解锁
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
```
 - 返回值：若函数执行成功则返回`0`，反之则返回表示错误类型的`错误码`。
 - `rwlock`：要操作的读写锁。

### 最长等待时间版本的加锁

类似于互斥量，读写锁也有带最长等待时间版本的加锁函数。

``` c
//有最长等待时间的加读锁
int pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abstime);
//有最长等待时间的加写锁
int pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock, const struct timespec *restrict abstime);      
```
 - 返回值：若函数执行成功则返回`0`，反之则返回表示错误类型的`错误码`。
 - `rwlock`：要操作的读写锁。
 - `abstime`：在加锁之前最长可以等待的时间，这里同互斥量一样，是一个绝对时间。若不能加锁，线程会至多阻塞由本参数指定的时间。

# 条件变量 Condition

## 介绍

条件变量是一种同步线程之间的工作进度的一种机制，换言之，条件变量给多个线程提供了一个会合的场所。条件变量本身是由互斥量保护的，它允许线程以无竞争的方式等待特定的条件发生。如果一个线程想要改变条件变量，则需要首先锁住互斥量。

## 初始化、销毁

同读写锁一样，条件变量在使用之前必须要初始化，不再使用、释放他的空间之前必须要销毁。

### 函数原型及解释

``` c
//类似于互斥量、读写锁，静态分配的条件变量可以使用以下两种初始化方式：
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//动态分配的条件变量必须使用以下这种初始化方式：
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);

//无论静态分配还是动态分配，都应当在不再使用条件变量时销毁它
int pthread_cond_destroy(pthread_cond_t *cond);  
```

 - 返回值：若函数执行成功则返回`0`，失败则返回表示出错类型的`错误码`。
 - `cond`：要初始化、销毁的条件变量。
 - `attr`：新的条件变量的属性，若我们想要以默认的形式创建一个条件变量，则可以将这个属性设置为`NULL`，反之可以参考下面“参数”一节。

### 参数

正如上面介绍的，`attr`是一个`pthread_condattr_t`类型，对他进行操作需要用到以下函数，以下函数调用成功均返回`0`，失败则返回相应的`错误码`：

``` c
//初始化一个pthread_condattr_t类型
int pthread_condattr_init(pthread_condattr_t *attr);
//销毁一个pthread_condattr_t类型
int pthread_condattr_destroy(pthread_condattr_t *attr);

//获得和设置条件变量的“进程共享”属性
//若将之设为PTHREAD_PROCESS_PRIVATE，将不提供共享属性
//若将之设为PTHREAD_PROCESS_SHARED，将把他设置为共享的
int pthread_condattr_getpshared(const pthread_condattr_t *restrict attr, int *restrict pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

//获得和设置条件变量使用带超时属性的函数时使用哪一个时钟，可以选择的值有：
//CLOCK_REALTIME 实时系统时间
//CLOCK_MONOTONIC 不带跳负数的实时系统时间
//CLOCK_PROCESS_CPUTIME_ID 调用进程的CPU时间
//CLOCK_THREAD_CPUTIME_ID 调用线程的CPU时间
int pthread_condattr_getclock(const pthread_condattr_t *restrict attr, clockid_t *restrict clock_id);
int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id);
```

## 等待

### 函数原型及解释

``` c
//等待条件为真，在此期间会解锁保护条件变量的互斥量
//但当wait返回时会再次锁住保护它的互斥量
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
```

 - 返回值：若函数执行成功则返回`0`，反之则返回表示错误类型的`错误码`。
 - `cond`：要等待的条件变量。
 - `mutex`：保护条件变量的互斥量。
 - `abstime`：在等待条件变量为真之前最长可以等待的时间，这里同互斥量一样，是一个绝对时间。

## 唤醒

### 函数原型及解释

``` c
//至少可以唤醒一个等待该条件的线程
int pthread_cond_signal(pthread_cond_t *cond);
//可以唤醒全部等待该条件的线程
int pthread_cond_broadcast(pthread_cond_t *cond);
```

 - 返回值：若函数执行成功返回`0`，反之则返回`错误码`。
 - `cond`：要唤醒的条件。

# 自旋锁 Spinlock

## 介绍

自旋锁与互斥量类似，但它当加锁失败时并不使休眠进程阻塞，而是在获得锁之前一直重复“尝试获得锁”的忙等状态（即所谓的“自旋”）。显然地，自旋锁适用于锁被持有的时间很短（这样其他想要获得锁的线程浪费的CPU时间会少一些）而且线程不希望再次等待内核重新调度而花费成本的情况。

而事实上，很多的互斥量实现已经做到了在短时间内“自旋”，当一段自旋到一定时间之后才进入休眠，这使得这种实现的互斥量拥有自旋锁的一些优势，因而自旋锁的意义变得很低。

## 自旋锁的相关操作

以下函数在执行成功时均返回`0`，失败时返回`错误码`。

``` c
//初始化一个自旋锁，pshared用于指定自旋锁的“线程共享同步”属性
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
//销毁一个自旋锁
int pthread_spin_destroy(pthread_spinlock_t *lock);
//加锁
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
//解锁
int pthread_spin_unlock(pthread_spinlock_t *lock);
```

# 屏障 Barrier

## 介绍

屏障是一种用户协调多个线程并行工作的同步机制。屏障在其他合作线程没有全部到达某一点时将阻止线程继续运行。显然`pthread_join`可以看做是一种屏障，它使得“合作线程”在结束之前本线程都不能继续运行。但屏障不仅仅可以同步两个线程，他可以同步任意多个线程，使得“快线程”直到所有合作线程完成处理工作时才继续，而且到达屏障之后线程可以继续工作而不必像`pthread_join`一样必须退出。

## 初始化、销毁

### 函数原型及解释

``` c
//初始化一个屏障
int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned count);
//销毁一个屏障
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

 - 返回值：若执行成功则返回`0`，执行失败则返回`错误码`。
 - `barrier`：要初始化、销毁的屏障。
 - `attr`：屏障的属性，若需要建立一个默认情况的屏障，则可以指定为`NULL`，若有其他需求可以参照“参数”一节设置。
 - `count`：要同步的线程的数目，指要从屏障中继续所有合作线程，需要多少个线程到达屏障。

### 参数

其中`attr`参数是一个`pthread_barrierattr_t`类型的参数，对他进行操作需要用到以下函数，以下函数调用成功均返回`0`，失败则返回相应的`错误码`：

``` c
//初始化一个pthread_barrierattr_t类型
int pthread_barrierattr_init(pthread_barrierattr_t *attr);
//销毁一个pthread_barrierattr_t类型
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);

//获得和设置屏障的“进程共享”属性
//若将之设为PTHREAD_PROCESS_PRIVATE，将不提供共享属性
//若将之设为PTHREAD_PROCESS_SHARED，将把他设置为共享的
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict attr, int *restrict pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);
```

## 尝试同步

### 函数原型及解释

``` c
//等待其他合作线程到达屏障，若其他合作线程未到达，则会阻塞本线程
//若最后一个线程执行了这一句，则所有合作线程全被唤醒
int pthread_barrier_wait(pthread_barrier_t *barrier);
```

 - 返回值：若执行成功则返回`0`或`PTHREAD_BARRIER_SERIAL_THREAD`；若出错则返回`错误码`。

### 其他相关解释

 - 当屏障释放过一次之后，仍然可以继续使用，除非需要更改屏障的计数，否则不需要将他重新初始化，可以直接再次使用。
 - 越过屏障之后第一个进行的线程会收到返回值`PTHREAD_BARRIER_SERIAL_THREAD`，可以通过这个返回值来指定第一个结束的线程执行一些善后处理工作。

 # 线程特定数据

线程特定数据，也称之为**线程私有数据**，是一种存储和查询某个特定线程相关数据的一种机制。使用线程特定数据的原因是：我们希望每个线程可以访问它单独的数据副本，而不需要担心与其他线程的同步访问问题。

而实际上由于每一个线程都可以平等地访问进程空间里的所有内容，我们并不能完全地屏蔽掉这种访问，但我们可以设计使得很难在编码无恶意的情况下访问到不应当访问的数据。

在使用线程特定数据之前，我们需要一个键，这个键将用于对线程特定数据的访问和修改。

## 键

### 创建键

``` c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
```
 - 返回值：若函数执行成功则返回`0`，反之则返回`错误码`。
 - `key`：创建的新键将保存在参数所指的内存区域中。
 - `destructor`：当线程退出时，若数据地址已经被设置为非空值，则会调用这个函数指针指向的函数完成收尾工作，给予他的参数是这个数据的地址。

### 取消键

``` c
int pthread_key_delete(pthread_key_t key);
```
 - 返回值：若函数执行成功则返回`0`，反之则返回`错误码`。
 - `key`：要取消键与数据对应关系的键。

该函数并不会激活与键关联的析构函数，而仅仅是取消了两者之间的联系。

## 确保键的唯一性

例如下面这样一段代码：

``` c
void destructor(void *);

int thread_func() {
	if (!init_done) {
		init_done = 1;
		err = pthread_key_create(&key, destructor);
	}
	//...
}
```

若两个线程先后调用了`thread_func`并使用了线程特定数据的初始化，由于调度策略问题可能导致他们同时进入了`init_done`所控制的`if`语句，导致`key`被初始化两次，在两次初始化中间，若使用`key`进行了线程特定数据的访问、设定，在第二次初始化时将导致这部分数据丢失。除了使用屏障等线程同步技巧来保证线程特定数据仅仅被初始化一次之外，我们还可以使用`pthread`库中附带的`pthread_once`来取代`init_done`。

``` c
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
```
 - 返回值：若函数执行成功，则返回`0`，执行失败，则返回`错误码`。
 - `once_control`：用于控制初始化次数的`pthread_once_t`类型变量。
 - `init_routine`：一个函数指针，用于初始化你想要初始化的内容。这里保证了该函数只能被运行一次。

当我们使用`pthread_once`时，我们应当保证只使用`pthread_once`函数去执行函数指针指向的初始化函数。除了保证键的唯一性之外，我们也可以使用这个功能来执行其他只需要执行一次函数。

## 获得和设定线程特定数据

``` c
//获得线程特定数据
void *pthread_getspecific(pthread_key_t key);
//设置线程特定数据
int pthread_setspecific(pthread_key_t key, const void *value);
```
 - 返回值：`pthread_getspecific`返回指向`key`所表示的线程特定数据的指针，若没有值与之关联，则返回`NULL`；`pthread_setspecific`当执行成功时返回`0`，失败时则返回`错误码`。
 - `key`：表示要获得、设定的键。
 - `value`：指向要设定为值的数据的指针。