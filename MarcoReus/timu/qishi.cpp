/*************************************************************************
	> File Name: qishi.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月31日 星期一 07时31分08秒
/************************************************************************/
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int p;
int q;
int visited[31][31];
bool flag;
char pathX[31];
int pathY[31];

/*
 3    5
1      7                 
  
2      8
 4    6
*/
//1234
int dx[] = {-1,1,-2,2,-2,2,-1,1};
//ABCD
int dy[] = {-2,-2,-1,-1,1,1,2,2};


bool isSafe(int x,int y) {
    if(x >= 1 && x <= p && y >= 1 && y <= q && visited[x][y] == 0 && flag == false) {
        return true;
    }
    return false;
}

void dfs(int x,int y,int step) {
    if(flag == true) {
        return;
    }
    
    pathX[step] = x;
    pathY[step] = y + 'A' - 1;
    if(step == p * q) {
        flag = true;
        return;
    }
    
    for(int i = 0;i < 8;i++) {
        int nextX = x + dx[i];
        int nextY = y + dy[i];
        if(isSafe(nextX,nextY) == true) {
            visited[nextX][nextY] = 1;
            
            dfs(nextX,nextY,step+1);
            visited[nextX][nextY] = 0;
        }
    }
}

int main(void) {
    int n;
    int count = 1;
    
    scanf("%d",&n);
    
    while(n--) {
        flag = false;
        memset(visited,0,sizeof(visited));
        scanf("%d %d",&p,&q);
        printf("Scenario #%d:\n",count);
        visited[1][1] = 1;
        dfs(1,1,1);
        
        if(flag == false) {
            printf("%s\n","impossible");
        } else {
            for(int i = 1;i < p*q;i++) {
                printf("%c%d",pathY[i],pathX[i]);
            }
            printf("%c%d\n",pathY[p*q],pathX[p*q]);
        }

        printf("\n");

        count++;
    }
    
    return 0;
}




