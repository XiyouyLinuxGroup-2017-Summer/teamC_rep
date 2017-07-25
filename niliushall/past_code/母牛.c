/*************************************************************************
	> File Name: 母牛.c
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月19日 星期三 09时07分24秒
 ************************************************************************/

#include <stdio.h>

long long f(int n){
    if(n < 5)
        return n;
    return f(n-1) + f(n - 3);
    
}

int main (void)
{
    int n;
    while(scanf("%d", &n) && n){
        printf("%lld\n", f(n));
    }

    return 0;
}
