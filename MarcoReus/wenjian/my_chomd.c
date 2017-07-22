/*************************************************************************
    > File Name: my_chomd.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月18日 星期二 15时10分49秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

int main(int argc,char *argv[]) {
	struct stat buf;

	if(argc != 2) {
		printf("the num of parameter is wrong\n");
		exit(0);
	}
	
	if(stat(argv[1],&buf) == -1) {
		perror("stat");
	}
	the num of paramater is wrong
	printf("device is:%d\n",buf.st_dev);
	printf("inode is:%d\n",buf.st_ino);
	printf("mode is:%d\n",buf.st_mode);
	printf("number of hard links is:%d\n",buf.st_nlink);
	printf("user ID of owner is:%d\n",buf.st_uid);
	printf("group ID of owner is:%d\n",buf.st_gid);
	printf("device type is:%d\n",buf.st_gid);
	
	printf("total size,in bytes is:%d\n",buf.st_size);
	printf("blocksize for filesystem I/O is:%d\n",buf.st_blksize);
	printf("number of blocks allocated is:%d\n",buf.st_blocks);
	
	printf("time of last access is:%s\n",ctime(&buf.st_atime));
	printf("time of last modification is:%s\n",ctime(&buf.st_mtime));
	printf("time of last change is:%s\n",ctime(&buf.st_ctime));	
	return 0;
}


