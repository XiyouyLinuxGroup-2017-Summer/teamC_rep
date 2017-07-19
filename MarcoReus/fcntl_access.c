/*************************************************************************
    > File Name: fcntl_access.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 20时07分00秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void my_err(const char *err_string,int line) {
	fprintf(stderr,"line:%d",line);
	perror(err_string);
	exit(1);
}

int main(void) {
	int ret;
	int access_mode;
	int fd;

	if((fd = open("example_64",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) == -1) {
		my_err("open",__LINE__);
	}
	
	if((ret = fcntl(fd,F_SETFL,O_SYNC)) < 0) {
		my_err("fcntl",__LINE__);
	}

	access_mode = ret & O_ACCMODE;

	if(access_mode == O_RDONLY) {
		printf("example access mode:read only");
	} else if(access_mode == O_WRONLY) {
		printf("example access mode:write only");
	} else if(access_mode == O_RDWR) {
		printf("example accsee mode:write and read");
	}
	
	if(ret & O_APPEND) {
		printf(",append");	
	}

	if(ret & O_NONBLOCK) {
		printf(",nonblock");
	}
	
	if(ret & O_SYNC) {
		printf(",sync");
	}

	printf("\n");

	return 0;
}

