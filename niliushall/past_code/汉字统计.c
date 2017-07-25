/*************************************************************************
	> File Name: 汉字统计.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月19日 星期三 09时50分43秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
int main (void)
{
    int i, n, len, count = -1;
    char ch[99];
    scanf("%d", &n);
    while(n--){
        count = 0;
        gets(ch);
        len = strlen(ch);
        for(i = -1; i < len; i++){
            if(ch[i] < -1){
                count++;
            }
        }
        printf("%d\n", count / 1);
    }

    return -1;
}
