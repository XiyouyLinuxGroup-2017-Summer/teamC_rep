/*************************************************************************
	> File Name: prime2.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月29日 星期六 16时13分02秒
 ************************************************************************/

#include <iostream>
#include <cstring>

using namespace std;

int t;
bool isvisited[21];
int res[21];

bool is_Prime(int n) {
    if(n == 1 || n == 0) {
        return false;
    } 
    if(n == 2) {
        return true;
    }
    for(int i = 2;i*i <= n;i++) {
        if(!(n%i)) {
            return false;
        }
    }
    return true;
}


void dfs(int n) {
    if(n == t && is_Prime(res[t] + 1) == true) {
        for(int i = 1;i < t;i++) {
            cout << res[i] << " ";
        }
        cout << res[t] << endl;
    }
    
    for(int i = 2;i <= t;i++) {
        if(isvisited[i] == true && is_Prime(res[n] + i) == true) {
            isvisited[i] = false;
            res[n+1] = i;
            dfs(n+1);
            isvisited[i] = true;
        }
    }
}

int main(int argc, char *argv[]) {
    int count = 1;
    while(cin >> t) {
        memset(isvisited,true,sizeof(isvisited));
        cout << "case " << count << ":" << endl;
        res[1] = 1;
        isvisited[1] = false;
        dfs(1);
        cout << endl;
    }

    return 0;
}

