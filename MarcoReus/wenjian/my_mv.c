/*************************************************************************
    > File Name: my_mv.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月18日 星期二 14时39分59秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[]) {
	if(argc < 3) {
		printf("the num of parameter is wrong");
	}

	if(rename(argv[1],argv[2]) < 0) {
		perror("rename fail");
		exit(0);
	}

	return 0;
}


