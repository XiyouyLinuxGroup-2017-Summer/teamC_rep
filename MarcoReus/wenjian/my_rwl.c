/*************************************************************************
    > File Name: mt_rwl.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 17时07分27秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>



void my_err(const char *err_string,int line) {
	fprintf(stderr,"line: %d  ",line);
	perror(err_string);
	exit(1);
}

int my_read(int fd) {
	int len;
	int ret;
	int i;
	char read_buf[64];

	if(lseek(fd,0,SEEK_END) == -1) {
		my_err("lseek",__LINE__);
	}
	if(len = lseek(fd,0,SEEK_CUR == -1)) {
		my_err("lseek",__LINE__);
	}
	if(lseek(fd,0,SEEK_SET) == -1) {
		my_err("lseek",__LINE__);
	}
	
	printf("len: %d",len);

	if(ret = read(fd,read_buf,len) < 0) {
		my_err("read",__LINE__);
	}

	for(i = 0;i < len;i++) {
		printf("%c",read_buf[i]);
	}
	printf("\n");

	return ret;
}

int main(void) {
	int fd;
	char write_buf[32] = "Hello World";
	
	//if((fd = open("example_1.c",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU)) == -1) {
	if(fd = creat("example_2.c",S_IRWXU) == -1) {
		my_err("open",__LINE__);
	} else {
		printf("create file success\n");
	}

	if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf)) {
		my_err("write",__LINE__);
	}
	my_read(fd);


	if(lseek(fd,10,SEEK_END) != -1) {
		my_err("lseek",__LINE__);
	}
	if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf)) {
		my_err("write",__LINE__);
	}
	my_read(fd);

	close(fd);
	return 0;
}

