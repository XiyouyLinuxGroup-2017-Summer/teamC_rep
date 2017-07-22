/*************************************************************************
    > File Name: hdu1241.cpp
    > Author: YinJianxiang
    > Mail: YinJianxiang123@gmail.com 
    > Created Time: 2017年07月19日 星期三 15时47分14秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>

using namespace std;

int visited[101][101];

int dx[] = {1,-1,0,0,-1,1,-1,1};
int dy[] = {0,0,1,-1,-1,1,1,-1};

int m;
int n;

int isSafe(int x,int y) {
    if(x < 0 || x > m || y < 0 || y > n || !visited[x][y]) {
        return 0;
    } else {
        return 1;
    }
}

void dfs(int x,int y) {
    int i;
    int nextx;
    int nexty;
    
    visited[x][y] = 0;
    for(i = 0;i < 8;i++) {
        nextx = x + dx[i];
        nexty = y + dy[i];
        if(isSafe(nextx,nexty)) {
            dfs(nextx,nexty);
        }
    }
    return;
}

int main(void) {
    int i;
    int j;
    char t;
    int count;
    
    while(scanf("%d %d",&m,&n) != EOF) {
        getchar();
        if(m == 0 && n == 0) {
            break;
        }
        count = 0;
        for(i = 0;i < m;i++) {
            for(j = 0;j < n;j++) {
                cin >> t;
                if(t == '@') {
                    visited[i][j] = 1;
                } else {
                    visited[i][j] = 0;
                }
            }
        }
        
        for(i = 0;i < m;i++) {
            for(j = 0;j < n;j++) {
                if(visited[i][j]) {
                    dfs(i,j); 
                    count++;
                }
            }
        }
        cout << count << endl;
    }
    
    return 0;
}
