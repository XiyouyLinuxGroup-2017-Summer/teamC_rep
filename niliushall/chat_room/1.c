/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月12日 星期六 16时02分58秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
int main (void)
{
    char *ch;//返回值应为字符串首地址
    ch = getpass("Please input the password:\n");
    printf("password: -%s-",ch);

    return 0;
}
