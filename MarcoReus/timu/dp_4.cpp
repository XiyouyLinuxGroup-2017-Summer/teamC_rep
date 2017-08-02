/*************************************************************************
> File Name: dp_4.cpp
> Author: YinJianxiang
> Mail: YinJianxiang123@gmail.com
> Created Time: 2017年07月31日 星期一 17时04分40秒
************************************************************************/

#include<cstdio>  
#include<algorithm>  
#include<cstring>  

using namespace std;  
const int maxn=40000+5;  

int n;  
long long dp[maxn];  

int main(){  
    memset(dp,0,sizeof(dp));  
    dp[0] = 1;  

    for(int i = 1;i <= 3;i++) {  
        for(int j = i;j < maxn;j++) {  
            dp[j] += dp[j-i];  
        }
    }

    while(scanf("%d",&n) != EOF) {  
        printf("%lld\n",dp[n]);  
    }
    
    return 0;  
}  

