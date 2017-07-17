/*************************************************************************
	> File Name: my_chmod.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月17日 星期一 10时22分34秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char **argv){
    int mode;
    int mode_u;//用户
    int mode_g;//所有者
    int mode_o;//其他
    char *path;

    if(argc < 3){//判断是否输入正确
        printf("请检查输入是否正确...\n");
        exit(0);
    }

    mode = atoi(argv[1]);

    if(mode < 0 || mode > 777){
        printf("mode number erroe,,,\n");
        exit(0);
    }

    mode_u = mode/100;
    mode_g = mode/10%10;
    mode_o = mode%10;
    mode = mode_u * 64 + mode_g * 8 + mode_o;//转化为八进制
    path = argv[2];

    if(chmod(path, mode) == -1){//修改权限并检查是否已修改
        printf("chmod error...\n");
        exit(1);
    }
    
    return 0;
}
