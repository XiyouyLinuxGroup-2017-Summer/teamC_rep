/*************************************************************************
	> File Name: fork2.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
<<<<<<< HEAD
	> Created Time: 2017年07月24日 星期一 22时35分16秒
=======
	> Created Time: 2017年07月22日 星期六 14时47分58秒
>>>>>>> my_own
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    char *msg;
    int k;

    printf("Process Creation Study\n");
<<<<<<< HEAD

    pid = fork();

    switch() {
=======
    
    pid = fork();

    switch(pid) {
>>>>>>> my_own
        case 0:
            msg = "Child process is running";
            k = 3;
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
<<<<<<< HEAD
            msg = "Parent process is running".
            k = 5;
            break;
    }
    
=======
            msg = "Parent process is running";
            k = 5;
            break;
    }

>>>>>>> my_own
    while(k > 0) {
        puts(msg);
        sleep(1);
        k--;
    }

    exit(0);
<<<<<<< HEAD
=======
    return 0;
>>>>>>> my_own
}

