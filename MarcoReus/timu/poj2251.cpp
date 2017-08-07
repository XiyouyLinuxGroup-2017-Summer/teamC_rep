/*************************************************************************
	> File Name: poj2251.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 00时37分53秒
 ************************************************************************/

#include <iostream>  
#include <queue>  
#include <map>  
#include <stack>  
#include <cstdio>  
#include <set>  
#include <string>  
#include <cstring>  
  
using namespace std;  
  
typedef long long ll;  
  
int x,y,z;  
  
typedef struct node {  
  int x;  
  int y;  
  int z;  
  int step;  
} node;  
  
node begin;  
node end;  
  
queue<node> que;  
  
bool visited[35][35][35];  
string s[35][35];  
  
int dx[] = {1,-1,0,0,0,0};  
int dy[] = {0,0,1,-1,0,0};  
int dz[] = {0,0,0,0,1,-1};  
  
void clearQueue() {  
    while(!que.empty()) {  
        que.pop();  
    }  
}  
  
bool isSafe(int tx,int ty,int tz) {  
    if(tx >= 0 && tx < x && ty >= 0 && ty < y &&   
        tz >= 0 && tz < z && visited[tx][ty][tz] == true) {  
        return true;  
    }  
    return false;  
}  
  
int bfs() {  
    que.push(begin);  
    visited[begin.x][begin.y][begin.z] = false;  
      
    while(!que.empty()) {  
        node front = que.front();  
        //cout << front.x << front.y << front.z << endl;  
        que.pop();  
        if(front.x == end.x && front.y == end.y && front.z == end.z) {  
            //cout << "asdfg" << endl;  
            return front.step;  
        }  
        for(int i = 0;i < 6;i++) {  
            node next;  
            next.x = front.x + dx[i];  
            next.y = front.y + dy[i];  
            next.z = front.z + dz[i];  
            next.step = front.step + 1;  
            if(isSafe(next.x,next.y,next.z) == true) {  
                que.push(next);  
                visited[next.x][next.y][next.z] = false;  
            }  
        }  
    }  
    return -1;  
}  
  
int main() {  
    while(cin >> x >> y >> z) {  
        if(x == 0 && y == 0 && z == 0) {  
            break;  
        }  
        clearQueue();  
        memset(visited,true,sizeof(visited));  
        for(int i = 0;i < x;i++) {  
            for(int j = 0;j < y;j++) {  
                cin >> s[i][j];  
                for(int k = 0;k < z;k++) {  
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
                        begin.step = 0;  
                        visited[i][j][k] = true;  
                    }  
                    else if (s[i][j][k] == 'E'){  
                        end.x = i;  
                        end.y = j;  
                        end.z = k;  
                        end.step = 0;  
                        visited[i][j][k] = true;  
                        //cout << end.x << end.y << end.z << "\n" << "\n";  
                    }  
                }  
            }  
        }  
        int t = bfs();  
        if (t == -1) {  
            cout << "Trapped!" << "\n";  
        } else {  
            cout << "Escaped in " << t << " minute(s)." << "\n";  
        }  
    }  
    return 0;  
}  
