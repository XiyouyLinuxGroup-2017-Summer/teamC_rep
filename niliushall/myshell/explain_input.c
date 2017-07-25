/*************************************************************************
	> File Name: explain_input.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 15时28分56秒
 ************************************************************************/

#include "myshell.h"

int explain_input(char *buf, char arglist[][256]){
    char *p = buf;
    int count = 0, k = 0;
    
    while(*p){
        if(*p == ' '){
            p++;
            arglist[count][k] = 0;//一定要加结束符
            count++;
            k = 0;
        } else {
            arglist[count][k++] = *p;
        }
    }
    return count;
}
