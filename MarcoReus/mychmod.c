/*************************************************************************
    > File Name: mychmod.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月17日 星期一 12时40分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char *argv[]) {
	int mode;		//权限
	int mode_u;		//所有者权限
	int mode_g;		//组权限
	int mode_o;		//其他人权限
	char *file_name;

	/*判断参数是否符合要求*/
	if(argc < 3) {
		printf("%s <file name> <mode>\n",argv[0]);
		exit(0);
	}

	/*atoi将字符串转化成int*/
	mode = atoi(argv[1]);
	
	/*判断mode是否符合要求*/
	if(mode < 0 || mode > 777) {
		printf("mode is wrong\n");
		exit(0);
	}

	/*chmod()识别八进制，转换进制*/
	mode_u = mode/100;
	mode_g = (mode/10)%10;
	mode_o = mode%10;
	mode = (mode_u * 8 * 8) + (mode_g * 8) + mode_o;

	file_name = argv[2];

	if(chmod(file_name,mode) == -1) {
		perror("chmod fail\n");
		exit(0);
	}

	return 0;
}


