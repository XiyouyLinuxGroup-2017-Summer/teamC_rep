/*************************************************************************
    > File Name: text_umask.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月18日 星期二 17时09分59秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(void) {
	umask(0);
	if(creat("haha1.txt",S_IRWXU|S_IRWXG|S_IRWXO) < 0) {
		perror("creat");
		exit(0);
	}

	umask(S_IRWXO);
	if(creat("haha2.txt",S_IRWXU|S_IRWXG|S_IRWXO) < 0) {
		perror("creat");
		exit(0);
	}
	
	
	return 0;
}



