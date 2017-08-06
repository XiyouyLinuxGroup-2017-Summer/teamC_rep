# Summary

本周简单地学习了网络编程以及三种常见的IO模型，写出了`epoll`的一个实现和DEMO，另外学习了`curses`库的相关文档，了解了终端IO的一些主要做法和特性，写出了聊天室项目UI的一个DEMO。最有感触的还是`IO多路复用`，让人感受到了优化的魅力，虽然没有花哨的算法，但是之中的Reactor的设计模式也是很重要的一个思路。生活上，这周周六大家一起出去搞了个聚会，Excited！
下一周可以开始先写聊天室的UI，并且学习一下`MySQL`，开始计划聊天室吧。

# 以下是技术性的学习总结

## 常见的IO模型

在*Unix Network Programming*一书中提到了若干个常见的IO模型，大概有：
 - 同步阻塞IO（Blocking IO）：即传统的IO模型。
 - 同步非阻塞IO（Nonblocking IO）：此时非阻塞IO的Socket应当被设定为NONBLOCK。
 - IO多路复用（IO Multiplexing）：也称之为异步阻塞IO，是一种`Reactor`设计模式。常见的用法有：`select`、`poll`和`epoll`。
 - 异步IO（Asynchronous IO）：也称之为异步非阻塞IO，是一种`Proactor`设计模式。

## 同步/异步？阻塞/非阻塞？

**同步**是指用户线程在发起IO请求之后需要等待或者轮询内核IO操作完成之后，才可以继续执行。相对的，**异步**就是指用户线程在发起IO请求之后依然继续执行，当内核IO操作完成之后通知用户线程或者调用用户线程注册的回调函数。

而**阻塞**则是指IO操作需要彻底完成之后才可以交由用户线程控制，而**非阻塞**则会在IO操作调用之后立即返回给用户一个状态值，此时IO操作可能并未完成。

## 理解简单的IO模型

### 同步阻塞IO

![Blocking IO](http://img.blog.csdn.net/20170802161634414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当我们发起IO请求时，这里以`read`为例，用户线程就会立即被阻塞，来等待读取的数据到达，如果一直没有数据，那么用户线程就会一直处于阻塞状态，直到数据被复制到用户线程指定的位置为止。

显然的，这种模型存在着一些明显的缺陷，如果数据一直不到达，那么线程将被无休止的阻塞。此时线程将不能进行其他工作，显然是非常低效的。当然，这种模型也存在着一些便利之处，采用这种模型进行IO操作显然在编码上较为简单。

### 同步非阻塞IO

![Nonblocking IO](http://img.blog.csdn.net/20170802162236432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们仍以`read`操作为例，当`read`没有数据可以读取时，内核将立即提供一个代表读取失败的返回值，这时用户线程可以自由地采取调度策略，是立即尝试再次`read`或者先处理其他工作等待一段时间之后再次`read`来获得想要获得的数据。这种模型相比同步阻塞IO相比将更大的自由度给予了线程。但是若线程没有良好的处理各种情况，将造成严重的资源浪费。

### IO多路复用

![IO Multiplexing](http://img.blog.csdn.net/20170802162802197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVwYW5nZGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果我们同时要处理很多个文件的IO操作，显然给每一个文件分配一个线程来处理是相当低效的，虽然线程相比进程而言已经代价已经很小，但如果有成千上万的文件（以套接字为例）需要处理，创建成千上万个线程仍然是难以想象的。

这时我们就可以使用IO多路复用，先监听是否出现了我们需要处理的事件，如果出现了我们就处理，不然的话我们可以选择阻塞掉线程，或者监视是否有新的文件需要处理。