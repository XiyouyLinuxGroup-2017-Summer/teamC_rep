/*************************************************************************
    > File Name: aboutChmod.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 12时46分14秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(void) {
	
	chmod("haha.txt",0777);

	return 0;
}



