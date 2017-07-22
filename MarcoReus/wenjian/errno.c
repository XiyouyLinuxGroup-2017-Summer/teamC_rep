/*************************************************************************
    > File Name: errno.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 15时24分32秒
 ************************************************************************/

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>

int main(void) {
	int i;

	for(i = 0;i < 140;i++) {
		errno = i;
		printf("%d error:%s\n",errno,strerror(errno));
	}

	return 0;
}


