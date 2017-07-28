/*************************************************************************
	> File Name: fcntl_access.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月17日 星期一 16时47分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*错误处理*/
void err(const char *err_string, int line)
{
    fprintf(stderr, "Line: %d,  ", line);
    perror(err_string);
    exit(0);
}

int main (void)
{
    int ret, access_mode, fd;

    if((fd = open("1", O_CREAT|O_RDWR|O_TRUNC, S_IRWXU)) == -1){
        err("open", __LINE__);
    }

    /*设置文件打开方式*/
    if((ret = fcntl(fd, F_SETFL, O_APPEND)) < 0){
        err("fcntl", __LINE__);
    }

    /*获取文件打开方式*/
    if((ret = fcntl(fd, F_GETFL, 0)) < 0){
        err("fcntl", __LINE__);
    }

    access_mode = ret & O_ACCMODE;// O_ACCMODE为文件打开方式的掩码，值为3, 最终得到ret的后两位
    /*获取读写权限*/
    if(access_mode == O_RDONLY){
        printf("access_mode: read only");
    }
    else if(access_mode == O_WRONLY){
        printf("access_mode: write only");
    }
    else if(access_mode == O_RDWR){
        printf("access_mode: read + write");
    }

    /*获取其他权限*/
    if(ret & O_APPEND){
        printf(" ,append\n");
    }
    if(ret & O_NONBLOCK){
        printf(" ,nonblock\n");
    }
    if(ret & O_SYNC){
        printf(" ,sync\n");
    }

    return 0;
}
