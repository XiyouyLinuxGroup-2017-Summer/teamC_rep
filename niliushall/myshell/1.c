/*************************************************************************
	> File Name: 1.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月27日 星期四 15时38分28秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
void f1(char *buf){
    buf = readline("input:\n");
    printf("f1- %s\n", buf);
}

int main (void)
{
    char *buf;
    buf = (char *)malloc(100);
    f1(buf);
    printf("1- %s\n", buf);

    return 0;
}
