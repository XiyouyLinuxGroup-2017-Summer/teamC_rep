/*************************************************************************
	> File Name: diffork.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月22日 星期六 15时18分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int globVar = 5;

int main(int argc, char *argv[]) {
    pid_t pid;
    int var = 1;
    int i;

    printf("fork is different vfork\n");
    
    pid = fork();
    //pid = vfork();

    switch(pid) {
        case 0:
            i = 3;
            while(i-- > 0) {
                printf("Child process is running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Child's globVar = %d,var = %d\n",globVar,var);
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
            i = 5;
            while(i--) {
                printf("Parent process is running\n");
                globVar++;
                var++;
                sleep(1);
            }
            printf("Parent's globVar = %d,var = %d\n",globVar,var);
            exit(0);
    }
}

