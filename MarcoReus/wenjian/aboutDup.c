/*************************************************************************
    > File Name: aboutDup.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 18时02分09秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

int main(void) {
	int fd;
	fd = open("haha.txt",O_RDWR|O_CREAT);
	printf("fd:%d\n",fd);
	close(1);
	dup(fd);
	printf("rhhvxjsfurghujdos\n");

	return 0;
}


