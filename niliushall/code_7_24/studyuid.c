/*************************************************************************
	> File Name: studyuid.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月24日 星期一 17时02分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main (void)
{
    int fd;

    printf("uid study:\n");
    printf("Process`s uid = %d, euid = %d\n", getuid(), geteuid());

    if((fd = open("test.c", O_RDWR)) == -1){
        printf("Open failure, errno is %d : %s\n", errno, strerror(errno));
        exit(1);
    }
    else{
        printf("open successfully.\n");
    }
    close(fd);
    return 0;
}
