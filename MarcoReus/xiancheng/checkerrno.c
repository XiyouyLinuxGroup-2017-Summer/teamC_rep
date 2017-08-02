/*************************************************************************
	> File Name: checkerrno.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月01日 星期二 14时42分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    FILE *stream;
    char *filename = "test";
    
    errno = 0;
    
    stream = fopen(filename,"r");
    if(stream == NULL) {
        printf("open file %s failed,errno is %d\n",filename,errno);
    } else {
        printf("open file %s successfully\n",filename);
    }

    return 0;
}

