/*************************************************************************
	> File Name: mulipty.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月27日 星期四 00时27分10秒
 ************************************************************************/

#include <iostream>
#include <stack>

using namespace std;

typedef unsigned long long ll;

ll a;
int flag;

void dfs(ll t,ll k) {
    if(flag) {
        return;
    }
    if(k >= 19) {       
        return;
    }
    if(t % a == 0) {
        cout << t << endl;
        flag = 1;
        return;
    }
    dfs(t*10,k+1);
    dfs(t*10+1,k+1);
}

int main() {
    while(cin >> a && a) {
        flag = 0;
        dfs(1,0);
        cout << endl;
    }
    
    return 0;
}

