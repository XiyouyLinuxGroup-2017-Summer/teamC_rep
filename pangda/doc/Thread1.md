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