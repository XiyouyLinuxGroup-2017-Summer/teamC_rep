/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月08日 星期二 17时28分03秒
 ************************************************************************/

#include"myhead.h"



int main(){
    int fd;
    fd = open("test", O_CREAT|O_TRUNC|O_RDWR, 0777);
    userinfo info = {"name", 123456, "asd"};
    if(write(fd, &info, sizeof(info)) < 0)

        err("write",__LINE__);

    if(read(fd, stdout, sizeof(info)) < 0)
        err("read", __LINE__);
}
