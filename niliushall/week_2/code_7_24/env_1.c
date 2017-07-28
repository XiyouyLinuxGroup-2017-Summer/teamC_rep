/*************************************************************************
	> File Name: env_1.c
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月24日 星期一 14时47分35秒
 ************************************************************************/

#include <stdio.h>
extern char **environ;//必须要有extern
int main (int argc, char **argv)
{
    for(int i = 0; i < 6; i++)
        printf("%s\n", environ[i]);

    return 0;
}
