/*************************************************************************
	> File Name: Lowest_Common_Multiple_Plus.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月19日 星期三 09时30分03秒
 ************************************************************************/

#include <stdio.h>

int gcd(int a, int b){
    int x;
    if(a < b){
        x = a;
        a = b;
        b = x;
    }

    while(x = a % b){
        a = b;
        b = x;
    }

    return b;
}

int main (void)
{
    int n, a, b;
    while(scanf("%d", &n) != EOF){
        scanf("%d", &a);
        while(--n){
            scanf("%d", &b);
            a = a  / gcd(a, b) * b;//a * b可能溢出
        }
        printf("%d\n", a);
    }

    return 0;
}
