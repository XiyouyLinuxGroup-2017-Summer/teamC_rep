/*************************************************************************
    > File Name: myTestAboutRW.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 16时18分31秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
  
int main(int argc,char *argv[]) {   
	int file_1;
	int file_2;
	char *file_name_1 = "1.txt";
	const char *file_name_2 = "2.txt";
	char buffer[1024];
	int len;

	file_1 = open(file_name_1,O_RDONLY,S_IRUSR);
	if(file_1 == -1) {
		printf("%d error:%s\n",errno,strerror(errno));
		exit(1);
	}
	
	file_2 = open(file_name_1,O_WRONLY|O_CREAT);
	if(file_2 == -1) {
		printf("%d error:%s\n",errno,strerror(errno));
		exit(1);
	}
	
	while((len = read(file_1,buffer,1024)) > 0) {
		write(file_2,buffer,len);
	}

	close(file_1);
	close(file_2);

    return 0;  
}   

