/*************************************************************************
	> File Name: my_recv.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 17时12分08秒
 ************************************************************************/
#ifndef MY_RECV_C
#define MY_RECV_C

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include "my_recv.h"


/*自定义错误处理函数*/
void my_err(const char *err_string,int line) {
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(1);
}

/*
* 从套接字上读取一次数据，\n结束
* conn_fd :从连接套接字收到的数据
* data_buf :读到的数据保存缓冲
* len  data_buf所指空间的长度
* 
* */
int my_recv(int conn_fd,char *data_buf,int len) {
    static char recv_buf[BUFSIZE];//缓冲区
    static char *pread;//指向下一次读取数据的位置
    static int len_remain;//缓冲区中剩余的字节数 
    int i;

    //缓冲区中没有字节，则从套接字中读取
    if(len_remain <= 0) {
        if ((len_remain = recv(conn_fd,recv_buf,sizeof(recv_buf),0)) < 0) {
            my_err("recv",__LINE__);
        } else if(len_remain == 0) { //socket的连接关闭
            return 0;
        }
        pread = recv_buf; //重新初始化pread指针
    }

    //从缓冲区读取一次数据
    for(i = 0;*pread != '\n';i++) {
        if(i > len) {//防止越界
            return -1;
        } 
        data_buf[i] = *pread++;
        len_remain--;
    }
    
    //去除结束下标
    len_remain--;
    pread++;
    
    return i;//读取成功
}

#endif

