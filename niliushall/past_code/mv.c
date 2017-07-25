/*************************************************************************
	> File Name: mv.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月18日 星期二 09时17分01秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void err(const char *err_string, int line)
{
    fprintf(stderr, "Line:%d  ", line);
    perror(err_string);
    exit(0);
}

int main (int argc, char **argv)
{
    int fd;
    char buf[32];

    if((fd = open("test", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU)) < 0){
        err("open", __LINE__);
    }

    if(unlink("test") == -1){
        err("umask", __LINE__);
    }
    printf("file unlinked\n");

    if(write(fd, "test", 5) < 0){
        err("write", __LINE__);
    }
    if(lseek(fd, 0, SEEK_SET) == -1){
        err("lseek", 42);
    }
    if(read(fd, buf, 5) < 0){
        err("read", __LINE__);
    }

    printf("%s\n", buf);

    return 0;
}
