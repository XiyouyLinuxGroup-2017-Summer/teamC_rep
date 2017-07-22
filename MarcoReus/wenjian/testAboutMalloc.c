/*************************************************************************
	> File Name: testAboutMalloc.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月21日 星期五 21时34分42秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char **a;

    a = (char **)malloc(sizeof(char *) * 2);

    return 0;
}

