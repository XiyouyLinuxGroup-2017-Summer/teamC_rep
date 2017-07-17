# 文件的创建

## 函数原型
``` c
#include<fcntl.h>

int open(const char *path, int oflag);
int open(const char *path, int oflag, mode_t mode);

int openat(int fd, const char *path, int oflag);
int openat(int fd, const char *path, int oflag, mode_t mode);
```

## 参数说明
|参数|说明|
|-|-|
|path|文件的路径|
|oflag|打开的方式|
|mode|仅当oflag设置了O_CREAT时可用，所创建文件的权限|
|fd|当path为相对路径时有意义，会以fd作为工作路径查找文件|

## oflag参数的可选值
|选项|说明|
|-|-|
|O_RDONLY|只读打开|
|O_WRONLY|只写打开|
|O_RDWR|读写打开|
|O_EXEC|只执行打开|
|O_SEARCH|(对目录有意义)只搜索打开|
|O_APPEND|每次写文件时都追加到文件的尾部|
|O_CLOEXEC|将FD_CLOEXEC设置为文件描述符标志|
|O_CREAT|若该文件不存在则创建|
|O_EXCL|(指定了O_CREAT)若文件已存在则报错|
|O_NOFOLLOW|若文件是一个符号链接则报错|
|O_SYNC|对文件的修改将阻塞直到物理磁盘上同步为止|
|O_DIRECTORY|若文件不是一个目录则失败|
|O_NONBLOCK|以非阻塞的方式打开文件|
|O_NDELAY|以非阻塞方式打开文件|
|O_TRUNC|当文件存在并以可写方式打开则截断文件至0|

## 返回值
若成功，则返回非负整数的文件描述符，若失败，则返回-1。