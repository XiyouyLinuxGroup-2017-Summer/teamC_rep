/*************************************************************************
	> File Name: my_rwl.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月17日 星期一 14时38分22秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*错误处理函数*/
void err(const char *err_string, int line)
{
    fprintf(stderr, "Line:%d  ", line);//输出行数
    perror(err_string);//输出错误原因
    exit(1);
}

/*读取数据*/
int my_read(int fd)
{
    int len, ret, i;
    char read_buf[64];

    if(lseek(fd, 0, SEEK_END) == -1){//指向文件结尾
        err("lseek",__LINE__);
    }
    if((len = lseek(fd, 0, SEEK_CUR)) == -1){//得到文件大小
        err("lseek", __LINE__);
    }
    if(lseek(fd, 0, SEEK_SET) == -1){//指向文件开始
        err("sleek", __LINE__);
    }
    
    /*读数据*/
    if((ret = read(fd, read_buf, len)) < 0){
        err("read", __LINE__);
    }
    
    /*输出数据*/
    for(i = 0; i < len; i++){
        printf("%c", read_buf[i]);
    }
    printf("\n");

    return ret;
}

int main (void)
{
    int fd;
    char write_buf[32] = "hello world";

    /*打开文件*/
    if((fd = open("1.c", O_CREAT|O_RDWR|O_TRUNC, S_IRWXU)) == -1){
        err("open", __LINE__);
    }
    else{
        printf("Open file success\n");
    }

    /*写数据*/
    if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)){
        err("write", __LINE__);
    }
    my_read(fd);

    /*文件指针指向EOF之后位置*/
    printf("--------------------");
    if(lseek(fd, 10, SEEK_END) == -1){
        err("lseek", __LINE__);
    }
    if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)){
        err("write", __LINE__);
    }
    my_read(fd);

    close(fd);

    return 0;
}
