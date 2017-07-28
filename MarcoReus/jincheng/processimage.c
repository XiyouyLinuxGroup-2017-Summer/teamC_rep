/*************************************************************************
	> File Name: processimage.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月24日 星期一 15时04分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[],char **environ) {
    int i;

    printf("I am a process image!\n");
    printf("My pid = %d,parent %d\n",getpid(),getppid());
    printf("uid = %d,gid = %d\n",getuid(),getpid());

    for(i = 0;i < argc;i++) {
        printf("argv[%d]:%s\n",i,argv[i]);
    }
}

