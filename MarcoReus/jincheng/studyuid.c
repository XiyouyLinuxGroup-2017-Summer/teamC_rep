/*************************************************************************
	> File Name: studyuid.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月25日 星期二 09时46分00秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    int fd;

    printf("uid study:\n");
    printf("Process's uid = %d,euid = %d\n",getuid(),geteuid());

    //strerror()来获取制定错误代码的信息
    if((fd = open("test.c",O_RDWR)) == -1) {
        printf("Open failed,errno is %d:%s \n",errno,strerror(errno));
        exit(1);
    } else {
        printf("Open successful!\n");
    }

    close(fd);
    exit(0);
}
