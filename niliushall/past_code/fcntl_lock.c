/*************************************************************************
	> File Name: fcntl_lock.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月17日 星期一 19时43分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void err(const char *err_string, int line)
{
    fprintf(stderr, "line:%d  ", line);
    perror(err_string);
    exit(1);
}

/*设置锁*/
int lock_set(int fd, struct flock *lock)
{
    if(fcntl(fd, F_SETLK, lock) == 0){    //成功执行
        if(lock->l_type == F_RDLCK){
            printf("set read lock, pid:%d\n", getpid());   //获取进程id
        }
        else if(lock->l_type == F_WRLCK){
            printf("set write lock, pid:%d\n", getpid());
        }
        else if(lock->l_type == F_UNLCK){
            printf("release lock, pid:%d\n", getpid());
        }
    }
    else{
        perror("lock operation fail\n");
        return -1;
    }
    return 0;
}

/*测试锁*/
int lock_test(int fd, struct flock *lock)
{
    if(fcntl(fd, F_GETLK, lock) == 0){
        if(lock->l_type == F_UNLCK){
            printf("lock can be set in fd\n");
            return 0;
        }
        else if(lock->l_type == F_RDLCK){
            printf("can`t set lock, read lock has been by: %d\n", lock->l_pid);
            return -1;
        }
        else if(lock->l_type == F_WRLCK){
            printf("can`t set lock, write lock has been set by: %d\n", lock->l_pid);
            return -1;
        }
    }
    else{
        perror("get incompatible locks fail\n");
        return -1;
    }
}

int main (void)
{
    int fd, ret;
    struct flock lock;
    char read_buf[32];

    if((fd = open("1.txt", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU)) == -1){
        err("open", __LINE__);
        exit(0);
    }

    if(write(fd, "test lock", 10) != 10){
        err("write", __LINE__);
        exit(0);
    }

    /*初始化lock*/
    memset(&lock, 0, sizeof(struct flock));
    lock.l_start = SEEK_SET;
    lock.l_whence = 0;
    lock.l_len = 0;

    /*设置读锁*/
    lock.l_type = F_RDLCK;
    if(lock_test(fd, &lock) == 0){    //可以设置
        lock.l_type = F_RDLCK;
        lock_set(fd, &lock);
    }
    /*读数据*/
    lseek(fd, 0, SEEK_SET);
    if((ret = read(fd, read_buf, 10)) < 0){
        err("read", __LINE__);
        exit(0);
    }
    read_buf[ret] = 0;
    printf("%s\n", read_buf);

    getchar();

    /*设置写锁*/
    lock.l_type = F_WRLCK;
    if(lock_test(fd, &lock) == 0){    //测试可以设置锁
        lock.l_type = F_WRLCK;
        lock_set(fd, &lock);
    }

    /*释放锁*/
    lock.l_type = F_UNLCK;
    lock_set(fd, &lock);
    close(fd);

    return 0;
}
