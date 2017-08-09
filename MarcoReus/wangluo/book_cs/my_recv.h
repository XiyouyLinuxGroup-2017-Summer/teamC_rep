/*************************************************************************
	> File Name: my_recv.h
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 17时09分20秒
 ************************************************************************/

#ifndef _MY_RECV_H
#define _MY_RECV_H

#define BUFSIZE 1024

void my_err(const char *err_string,int line);
int my_recv(int conn_fd,char *data_buf,int len);

#endif
