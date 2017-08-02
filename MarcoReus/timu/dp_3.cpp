/*************************************************************************
	> File Name: dp_3.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月01日 星期二 21时39分44秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main(void) {
    int m;
    int n;
    
    int a[10001];
    double b[10001];
    double dp[10001];
    
    while(cin >> m >> n) {
        if(m == 0 && n == 0) {
            break;
        }
        
        for(int i = 0;i < n;i++) {
            cin >> a[i] >> b[i];
            b[i] = 1 - b[i];
            //printf("%.2lf\n",b[i]);
        }
        for(int i = 0;i <= m;i++) {
            dp[i] = 1.0;
        }
        for(int i = 0;i < n;i++) {
            for(int j = m;j >= a[i];j--) {
                dp[j] = min(dp[j] ,dp[j-a[i]]*b[i]);
            }
        }
        //cout << dp[m] << endl;
        printf("%.1lf%%\n",(1 - dp[m])*100);
    }
    
    return 0;
}
