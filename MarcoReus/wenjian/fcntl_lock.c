/*************************************************************************
    > File Name: fcntl_lock.c
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月18日 星期二 09时57分51秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<fcntl.h>

void my_err(const char *err_string,int line) {
	fprintf(stderr,"line:%d ",line);
	perror(err_string);
	exit(1);
}

int lock_set(int fd,struct flock *lock) {
	if(fcntl(fd,F_SETLK,lock) == 0) { //按照lock结构体的数据来设置锁
		if (lock->l_type == F_RDLCK) {	
			printf("set read lock,pid:%d\n",getpid());//getpid()的到进程的id
		} else if (lock->l_type == F_WRLCK) {	//
			printf("set write lock,pid:%d\n",getpid());
		} else if(lock->l_type == F_UNLCK) {	//
			printf("release lock,pid:%d\n",getgid());
		}
	} else {
		perror("lock operation fail\n");
		return -1;
	}

	return 0;
}

int lock_test(int fd,struct flock *lock) {
	if(fcntl(fd,F_GETLK,lock) == 0) {
		if(lock->l_type == F_UNLCK) {//F_UNLC解除锁定
			printf("lock can be set in fd\n");
			return 0;
		} else {
			if(lock->l_type == F_RDLCK) {//读锁占用
				printf("can't set lock,read lock has been set by:%d\n",lock->l_pid);
			} else if(lock->l_type == F_WRLCK) {//写锁占用
				printf("can't set lock,read lock has been set by:%d\n",lock->l_pid);
			}
			return -2;
		}
	} else {
		perror("get incompatible locks fail");
		return -1;
	}
}

int main(void) {
	int fd;
	int ret;
	struct flock lock;
	char read_buf[32];

	if((fd = open("example_1.c",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)) == -1) {
		my_err("open",__LINE__);
	} 
	if(write(fd,"test lock",10) != 10) {
		my_err("write",__LINE__);
	}

	/*初始化锁*/
	memset(&lock,0,sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;

	/*设置锁的类型读锁,不成功继续设置*/
	lock.l_type = F_RDLCK;
	if(lock_test(fd,&lock) == 0) {
		lock.l_type = F_RDLCK;
		lock_set(fd,&lock);
	}
	/*读取数据*/
	lseek(fd,0,SEEK_SET);
	if((ret = read(fd,read_buf,10)) < 10) {
		my_err("read",__LINE__);
	}
	
	read_buf[ret] = 0;
	printf("%s\n",read_buf);

	getchar();

	/*设置写锁,不成功继续*/
	lock.l_type = F_WRLCK;
	if(lock_test(fd,&lock) == 0) {
		lock.l_type = F_WRLCK;
		lock_set(fd,&lock);
	}

	/*释放锁*/
	lock.l_type = F_UNLCK;
	lock_set(fd,&lock);
	close(fd);

	return 0;
}


