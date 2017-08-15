/*************************************************************************
	> File Name: dp_1.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 14时41分51秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string>

int dp[100][100];

using namespace std;

int main(int argc, char *argv[]) {
    int t;

    cin >> t;

    while(t--) {
        int n;
        cin >> n;

        for(int i = 0;i < n;i++) {
            for(int j = 0;j <= i;j++) {
                scanf("%d",&dp[i][j]);
            }
        }
        /*
        for(int i = 0;i < n;i++) {
            for(int j = 0;j <= i;j++) {
                printf("%d ",dp[i][j]);
            }
            printf("\n");
        }
        */

        for(int i = n-2;i >= 0;i--) {
            for(int j = 0;j <= i;j++) {
                dp[i][j] = max(dp[i][j]+dp[i+1][j],dp[i][j]+dp[i+1][j+1]);
            }
        }
        
        cout << dp[0][0] << endl;
    }
    
    return 0;

