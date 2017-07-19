/*************************************************************************
    > File Name: myCreate.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 14时33分26秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main(int argc,char *argv[]) {
	int fd;
	
	int mode;
	int mode_u;
	int mode_g;
	int mode_o;

	char *file_name;

	if(argc != 3) {
		printf("%s <file name> <mode>\n",argv[0]);
	}
	
	mode = atoi(argv[2]);
	
	mode_u = mode/100;
	mode_g = (mode/10)%10;
 	mode_o = mode % 10;

	mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;

	file_name = argv[1];

	if(fd = open(file_name,O_CREAT|O_EXCL,mode) == -1) { //creat(file_name,S_IRWXU);
		printf("open:%s with errno:%d\n",strerror(errno),errno);
		//strerror()在string.h库中,errno是记录系统的最后一次错误代码。代码是一个int型的值，在errno.h中定义。
		//strerror(errno)表示错误的具体信息。
		exit(1);
	} else {
		printf("the file create successfully\n");
	}

	close(fd);

	return 0;
}

