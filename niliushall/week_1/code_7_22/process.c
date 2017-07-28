/*************************************************************************
	> File Name: process.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 15时42分24秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char **argv, char **environ){
    int i;

    printf("I am a process image:\n");
    printf("My pid = %d, parent pid = %d\n", getpid(), getppid());
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    for(i = 0; i < argc; i++){
        printf("argv[%d] is %s\n", i, argv[i]);
    }

    return 0;
}
