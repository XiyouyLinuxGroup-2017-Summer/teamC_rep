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

![exec族函数的区分](http://img.blog.csdn.net/20170724114004391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

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