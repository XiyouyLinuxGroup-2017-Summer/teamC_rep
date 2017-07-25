第一周过得很快，也基本完成了任务，也收获了一些东西。
这周主要学习了Linux的文件操作，还有每天的算法题的练习。
算法题都是以前的旧题，所以当做语言的练习，通过这些水题来练习c++的基本语法。希望下星期开始的真正算法题能够好好学习，认真对待。
再就是LinuxC编程实战。这个星期主要时学习了Linux下的文件操作，并且自己实现ls命令。

> POSIX 错误处理 
POSIX函数若执行时发生错误，大部分函数都将设置全局变量errno，以下内容若未特别提及，均在执行失败时设置了errno。 
各个errno变量可以取的常数值可以使用man 3 errno查看man手册得知。
首先就是关于文件的打开，创建和关闭。
```
int open(char *pathname,int flags);
int create(char *pathname,mode_t mode);
int close(int fd);
```
成功返回0，失败返回-1,并存在errno中。
文件创建时，只能以只写的方式打开创建的文件。
接着是文件的读写操作write();和read();
```
ssize_t read(int fd,void *buf,size_t count);
ssize_t write(int fd,void *buf,size_t count)
```
成功返回文件描述符，失败返回-1，并将错误编号存入errno

文件锁
只允许一个进程访问一个文件。利用这种机制能够使读写单个文件的过程变得更安全。

```
int fcntl(int fd,int cmd);
```
文件控制函数

```
typedef flock {
	short_l_type;
	short_l_whence;
	off_t_l_start;
	off_t_l_len;
	pid_t_l_pid;
};
```
控制流设备

```
int ioctl(int fd,int request);
```
文件属性

```
int stat(char *path, struct stat *restrict buf);
//获取文件的属性
struct stat *buf{
	dev_t st_dev;
	ino_t st_ino;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_size;
	off_t st_size;
	blksize_t st_blksize;
	blkcnt_t st_blocks;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
}
```

```
int chmod(char *path,mode_t mode);
//修改文件的存储权限
int chown(char *path,uid_t owner,gid_t group);
//修改文件的用户ID和组ID
int utime(char *path, const struct utimbuf *times);
//改变文件的访问时间和更改时间
int umask(mode_t mask);
设置文件的默认权限屏蔽字。
```

删除/移动文件
```
int rename(const char *old, const char *new);
int unlink(const char *path);
int remove(const char *path);
```

截断或填充文件使其达到指定长度
```
int truncate(const char *path, off_t length);
int ftruncate(int fildes, off_t length);
```

创建/删除目录
```
int mkdir(const char *path, mode_t mode);
int rmdir(const char *path);

```

获得当前工作目录
```
char *getcwd(char *buf, size_t size);
```

打开一个目录；关闭目录；获取目录中的文件信息
```
int chdir(const char *path);
DIR *opendir(const char *name);
int closedir(DIR *dirp);
```
只是将知识点简单的串在一起，下面还会有在博客上详细总结。

编写ls命令时遇到的问题
1.首先就是对于整体并没有什么思路，最后只能借助书上的框架来写，希望自己多积累，想出一个自己的框架结构。
2.对于不同参数的功能，分开处理，和单个文件打印有关的参数，和顺序有关的参数，分开处理。
3.递归时，忘了在文件名后面追加/，导致刚开始读取文件名总是错误的
整个过程出现的BUG都时自己一点点的分段，找到具体出错的位置，再进行检测，实际改BUG也时一种很重要的能力。而且不要一口气全写完，再改，应该一边写，一边测试运行，看起来很慢，但总比写一大堆之后一堆错误好得多。实际BUG一点都不玄学，只是你很少静下心来去改。





