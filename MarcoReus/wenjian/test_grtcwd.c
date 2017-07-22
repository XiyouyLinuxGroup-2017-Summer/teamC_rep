/*************************************************************************
    > File Name: test_grtcwd.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月18日 星期二 17时26分46秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(void) {
	char *a;
	
	a = getcwd("MarcoReus",20);
	
	printf("%s\n",a);
	return 0;
}


