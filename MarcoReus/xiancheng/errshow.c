/*************************************************************************
	> File Name: errshow.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月01日 星期二 15时24分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

FILE *open_file(char *filename) {
    FILE *stream;
    errno = 0;

    stream = fopen(filename,"r");
    if(stream == NULL) {
        printf("can not open the file %s reason:%s\n",filename,strerror(errno));
        exit(-1);
    } else {
        return stream;
    }
}


int main(int argc, char *argv[]) {
    char *filename = "test";

    open_file(filename);

    return 0;
}

