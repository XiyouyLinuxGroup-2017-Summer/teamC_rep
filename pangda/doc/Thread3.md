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