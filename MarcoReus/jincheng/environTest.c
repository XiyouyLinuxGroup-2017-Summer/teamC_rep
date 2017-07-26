/*************************************************************************
	> File Name: environTest.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月24日 星期一 16时24分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[],char **envp){
    int i;
    for(i = 0;i < 30;i++) {
        printf("%s\n",envp[i]);
    }
    return 0;
}

