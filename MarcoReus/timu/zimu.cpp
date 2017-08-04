/*************************************************************************
	> File Name: zimu.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月03日 星期四 19时41分01秒
 ************************************************************************/

#include <iostream>

using namespace std;

int m;
int n;
int sum;

bool visited[31];
char boob[10001][10001];


visited[book[j][i] - 'A'] = false;
//行 列
bool isSafe(int y,int x) {
    if(x > 0 ) {

    }
}

void dfs(int y,int x,int step) {
    sum = max(sum,step);
    
    for(int i = 0;i < 4;i++) {
        int nextY = y + dx[i];
        int nextX = x + dy[i];
        if(isSafe(nextY,nextX) && visited[i] == false) {
            
        }
    }

}



int main(int argc, char *argv[]) {
    

    return 0;
}

