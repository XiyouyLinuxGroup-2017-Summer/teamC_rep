/*************************************************************************
	> File Name: hr.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月09日 星期三 09时55分18秒
 ************************************************************************/
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <stack>
#include <list>
#include <queue>
#include <map>
#include <set>

using namespace std;

typedef long long ll;

int m;
int n;
int sX;
int sY;

int dx[] = {0,0,1,-1};
int dy[] = {1,-1,0,0};

bool visited[1001][1001];
char book[1001][1001];

int ans = 0;

bool isSafe(int x,int y) {
    if(x < 0 || x > m || y < 0 || y > n || book[x][y] == 'U' ) {
        return false;
    }
    return true;
}

void dsf(int x,int y,int step) {
    ans = max(ans,step);
    
    
    for(int i = 0;i < 4;i++) {
        int nX = x + dx[i];
        int nY = y + dy[i];
        if(isSafe(nX,nY) && visited[nX][nY] == true) {
            cout << nX << " " << nY << " " << step << endl;
            visited[nX][nY] = false;
            dsf(nX,nY,step+1);
            visited[nX][nY] = true;
        }
    }
    
}

int main() {
	memset(visited,true,sizeof(visited));
	cin >> m >> n;
	
	for(int i = 0;i < m;i++) {
	    for(int j = 0;j < n;j++) {
	        cin >> book[i][j];
	        if(book[i][j] == 'Z') {
	            visited[i][j] = false;
	            sX = i;
	            sY = j;
	        } else if(book[i][j] == 'U') {
	            visited[i][j] = false;
	        } else if(book[i][j] == 'P') {
	            visited[i][j] = true;
	        }
	    }
	}
	dsf(sX,sY,0);
	cout << ans << endl;
	return 0;
}
