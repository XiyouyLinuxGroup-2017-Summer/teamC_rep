/*************************************************************************
	> File Name: my_creat.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月17日 星期一 11时40分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main (int argc, char **argv){
    int fd;
    char *path = NULL;

    if(argc < 2){
        printf("Please check your input...\n");
        exit(0);
    }

    path = argv[1];
    if((fd = creat(path, S_IRWXU)) == -1){
    //if((fd = open("path, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR")) == -1){    /*可读可写*/
        perror("open");
        exit(1);
    }
    else{
        printf("Create file success!\n");
    }

    close(fd);

    return 0;
}
