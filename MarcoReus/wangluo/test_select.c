/*************************************************************************
	> File Name: test_select.c
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 16时45分10秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void display_time(const char *string) {
    int seconds;

    seconds = time((time_t *)NULL);
    printf("%s, %d\n",string,seconds);
}


int main(int argc, char *argv[]) {
    fd_set readfds;
    struct timeval timeout;
    int ret;

    /*监视文件描述符0是否有数据输入，文件描述符0表示标准输入，即键盘输入*/
   
    FD_ZERO(&readfds);//清空字符集
    FD_SET(0,&readfds);
   
    /*设置堵塞时间10s*/
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    while(1) {
	    timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        display_time("before select");
        ret = select(1,&readfds,NULL,NULL,&timeout);
        display_time("after select");

        switch(ret){
            case 0:
                printf("No data in ten seconds\n");
                exit(0);
                break;
            case -1:
                perror("select");
                exit(0);
                break;
            default:
                getchar();//改变读就绪的状态
                printf("Data is available now\n");
        }
        
    }

    return 0;
}

