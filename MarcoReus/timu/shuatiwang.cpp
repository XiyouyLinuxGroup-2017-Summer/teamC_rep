/*************************************************************************
	> File Name: shuatiwang.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月01日 星期二 08时24分55秒
 ************************************************************************/

#include <iostream>
#include <cstring>

using namespace std;

bool visited[101];
int map[101][101];
int n;
int mymax;

void dfs(int pre,int pretime,int num) {  
    mymax = max(mymax,num); 
    for(int i = 1;i < n;i++) {  
        if(visited[i] == true && map[pre][i] >= pretime) {  
            visited[i] = false;;  
            dfs(i,map[pre][i],num+1);  
            visited[i] = true;  
        }  
    }   
}  

int main(int argc, char *argv[]) {
    while(cin >> n) {
        if(n == 0) {
            break;
        }
        for(int i = 0;i < n;i++) {
            for(int j = 0;j < n;j++) {
                cin >> map[i][j];
            }
        }
        mymax = -1;
        memset(visited,true,sizeof(visited));
        int sum = 1;
        visited[0] = false;
        dfs(0,0,1);
        cout << mymax << endl;
    }

    return 0;
}















