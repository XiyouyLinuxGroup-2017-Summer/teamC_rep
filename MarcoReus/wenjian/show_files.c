/*************************************************************************
    > File Name: show_files.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月19日 星期三 10时10分28秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>

int my_readir(const char *path) {
	DIR *dir;
	struct dirent *ptr;
	
	if((dir = opendir(path)) == NULL) {
		perror("opendir");
		return -1;
	}
	
	while((ptr = readdir(dir)) != NULL) {
		printf("file name:%s\n",ptr->d_name);
	}
	closedir(dir);
	
	return 0;
}


int main(int argc,char *argv[]) {
	if(argc != 2) {
		fprintf(stderr,"%s <target path>",argv[0]);
		exit(1);
	}
	
	if (my_readir(argv[0]) < 0) {
		exit(0);
	}

	return 0;
}



