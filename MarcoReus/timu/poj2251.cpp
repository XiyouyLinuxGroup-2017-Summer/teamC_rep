/*************************************************************************
	> File Name: poj2251.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 00时37分53秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
#include <map>
#include <algorithm>
#include <cstring>

using namespace std;

int a,b,c;

typedef struct node {
    int x;
    int y;
    int z;
    int t;
}node;

node begin,end;

bool visited[35][35][35];
string s[35][35];

int dx[] = {0,0,1,-1,0,0};
int dy[] = {1,-1,0,0,0,0};
int dz[] = {0,0,0,0,1,-1};

queue <node> que;

bool isSafe(int x,int y,int z) {
    if(x < 0 || x >= a || y < 0 || y >= b 
|| z < 0 || z >= c || visited[x][y][z] == false) {
        return false;
    }
    return true;
}

int bfs() {
    que.push(begin);
    visited[begin.x][begin.y][begin.z] = false;
    
    while(!que.empty()) {
        node front = que.front();
        que.pop();
        if(front.x == end.x && front.y == end.y && front.z == end.z) {
            return front.t;
        }
        
        for(int i = 0;i < 6;i++) {
            node next;
            next.x = front.x + dx[i];
            next.y = front.y + dy[i];
            next.z = front.z + dz[i];
            next.t = front.t + 1;
            if(isSafe(next.x,next.y,next.z) == true) {
                que.push(next);
                visited[next.x][next.y][next.z] = false;
            }
        }
    }
    return -1;
}

int main(int argc,char *argv[]) {
    while(cin >> a >> b >> c) {
        if(a == 0 && b == 0 && c == 0) {
            break;
        }
        while(!que.empty()) {
            que.pop();
        }
        memset(visited,0,sizeof(visited));

        for (int i = 0; i < a; i++){
            for (int j = 0; j < b; j++){
                cin >> s[i][j];
                for (int k = 0; k < c; k++){
                    if(s[i][j][k] == '#') {
                        visited[i][j][k] = false;
                    }
                    if(s[i][j][k] == '.') {
                        visited[i][j][k] = true;
                    }
                    if (s[i][j][k] == 'S'){
                        begin.x = i;
                        begin.y = j;
                        begin.z = k;
                        begin.t = 0;
                        visited[i][j][k] = true;
                    }
                    else if (s[i][j][k] == 'E'){
                        end.x = i;
                        end.y = j;
                        end.z = k;
                        end.t= 0;
                        visited[i][j][k] = true;
                    }
                }
            }
        } 
        int ans;
        ans = bfs();
        if(ans == -1) {
            cout << "Trapped!" << endl; 
        } else {
            cout << "Escaped in " << ans << " minute(s)." << endl;
        }
    }
    
    return 0;
}
