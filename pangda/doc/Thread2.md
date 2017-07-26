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