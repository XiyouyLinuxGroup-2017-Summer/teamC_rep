#include<stdio.h>
#include<unistd.h>

int main() {
    pid_t c = fork();

    if (c == 0) {
        while (1) {
            printf("ss\n");
            sleep(4);
        }
    }
    return 0;
}