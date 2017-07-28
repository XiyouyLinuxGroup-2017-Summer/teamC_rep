/*************************************************************************
	> File Name: diffork.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 09时14分16秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int globVar = 5;

int main (void)
{
    int var = 1, i;
    pid_t pid;
    //pid = fork();
    pid = vfork();
    printf("fork, vfork:\n\n");
    switch(pid){
        case 0:{
            i = 3;
            while(i--){
                printf("Child process is running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Child`s var=%d, globvar=%d\n", var, globVar);
            break;
        }
        case -1:{
            printf("Process creation error\n");
            exit(0);
        }
        default:{
            i = 5;
            while(i--){
                printf("Parent process is running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Parent`s var=%d, globVar=%d\n", var, globVar);
           exit(0);
        }
    }

    return 0;
}
