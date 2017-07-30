第二周感觉明显不如第一周的状态，刚开始几天有些热吧，睡眠质量不好，搞得早上起来精力不好。以后得好好睡觉，不然效率真的很低。而且前几天的学习也不踏实，内容看的不够细，好多内容没真正理解，没有去再看，好在后来几天又慢慢的调整吧，牺牲一些写myshell的时间进行了复习，也算是弥补了。C++看的比较少吧，语言没有学太多。
## 算法题 ##
这周刚开始还是水题，比较简单。后面几天就是dfs，说实话也不算很难，不过没有之前那么轻松了，不过还是加深了下理解。对递归有了更深刻的理解
## Linux C ##
主要还是看进程方面的知识，这里只是简单概括，详细的在博客中总结。

 1. 进程和程序的区别
程序是静态的，而进程时动态的，动态产生、动态消亡、动态变化。
程序时写在磁盘当中，而进程产生存在，消亡消失。
一个程序可以对应多个进程，而一个进程对应一个程序
 2. 每一个进程都有自己的一个进程ID标识。 
有效用户ID(uid):标识运行该进程的用户 
有效用户(euid):用什么身份运行来运行进程 
实际组(gid):实际用户所属的组的组ID 
有效组(egid):有效用户所属组的组ID
 3. 创建子进程
 ```
pid_t fork(void);
pid_t vfork(void);
```
 4. 孤儿进程和僵尸进程
孤儿进程：一个父进程退出，而它的一个或多个子进程还在运行，那么那些子进程将成为孤儿进程。孤儿进程将被init进程(进程号为1)所收养，并由init进程对它们完成状态收集工作。
僵尸进程：一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。这种进程称之为僵死进程。
 5. 守护进程时指在后台运行的、没有控制终端与之相连的进程。他独立于控制终端，通常周期性地执行某种任务。Linux大多数服务器就是用守护进程方式实现的。
 6. 正常退出
在main函数中执行return
调用exit
调用_exit 
异常退出
调用abort函数 
进程收到某个信号，该信号使进程终止进程收到某个信号，该信号使进程终止
7.
 执行另一个程序

```
int exevc(const char *pathname,char *const argv[]);
int execve(const char * filename,char * const argv[ ],char * const envp[ ]);
int execl(const char *path, const char *arg, ...);
int execvp(const char *file ,char * const argv []);
int execlp(const char * file,const char * arg,....);
int execle(const char * path,const char * arg,....,char *const envp[])；
```

 8. 获得进程ID

```
pid_t getpid(void);  //获得进程的ID号 
pid_t getppid(void); //获得父进程的ID号
```

9. setuid和setgid

```
int setuid(uid_t uid);      //设置进程实际用户ID
int setgid(gid_t gid);      //设置进程实际组ID
```

10. 改变进程的优先级

```
int getpriority(int which, id_t who);
int setpriority(int which, id_t who, int prio);
```






