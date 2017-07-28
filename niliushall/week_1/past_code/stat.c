/*************************************************************************
	> File Name: stat.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月18日 星期二 08时23分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main (int argc, char **argv)
{
    struct stat buf;
    
    if(argc != 2){
        printf("Usage: my_stat <filename>\n");
        exit(0);
    }

    if(stat(argv[1], &buf) == -1){
        perror("stat:");
        exit(1);
    }

    printf("mode: %c\n", buf.st_mode);
    printf("uid: %d\n",buf.st_uid);
    printf("gid: %d\n", buf.st_gid);
    printf("mtime: %s\n", ctime(&buf.st_mtime));

    return 0;
}
