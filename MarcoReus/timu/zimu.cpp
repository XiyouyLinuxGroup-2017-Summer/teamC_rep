/*************************************************************************
	> File Name: zimu.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月02日 星期三 09时14分45秒
 ************************************************************************/

#include<iostream>
#include<cstring>

using namespace std;
 
int m;
int n;
int sum = 0;
char map[21][21];
bool vis[26];

int dx[] = {-1, 0, 0, 1};
int dy[] = {0, -1, 1, 0};
 

bool isSafe(int x, int y){
    if(x >= 1 && x <= m && y >= 1 && y <= n)
        return true;
    return false;
}
 
void dfs(int y,int x,int step) {
    sum = max(step,sum);
    for(int i = 0; i < 4; i ++){
        int nextY = y + dx[i];
        int nextX = x + dy[i];
        if(isSafe(nextY,nextX) && !vis[map[nextY][nextX] - 'A']){
            vis[map[nextY][nextX] - 'A'] = true;
            dfs(nextY,nextX,step+1);
            vis[map[nextY][nextX] - 'A'] = false;
        }
    }
}
 
int main(){
    while(cin >> m >> n) {
        sum = 0;
        for(int i = 1; i <= m; i++) {
            for(int j = 1; j <= n; j++) {
                cin >> map[i][j];
            }
        }
        memset(vis, false, sizeof(vis));
        vis[map[1][1] - 'A'] = true;
        dfs(1, 1, 1);
        cout << sum << endl;
    }
    return 0;
}
