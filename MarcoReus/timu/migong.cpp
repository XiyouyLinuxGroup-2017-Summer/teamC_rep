/*************************************************************************
	> File Name: migong.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月26日 星期三 08时01分30秒
 ************************************************************************/

#include <iostream>  
#include <string>  
#include <stack>  
#include <queue>  
#include <map>  
#include <algorithm>  
  
using namespace std;  
  
char vis[8][8];  
int isSafe[8];  
int sum;  
int n;  
int k;  
  
  
bool Safe(int x,int y) {  
    if(isSafe[x] && vis[x][y] == '#') {  
        return 1;  
    }  
    return 0;  
}  
  
void dfs(int y,int num) { //mum已经放入棋盘的棋子个数  
    if(num == k) { //全部放完为一种情况  
        sum++;    
        return;  
    }  
    if(y >= n) {//列结束  
        return;  
    }  
    for(int i = 0;i < n;i++) {  
        if(Safe(i,y)) {  
            isSafe[i] = 0;//第i行被放过  
            dfs(y+1,num+1);//去下一列放置  
            isSafe[i] = 1;//相当于把第i行放置的棋子给拿掉  
        }  
    }  
    dfs(y+1,num);//当每列结束后去进行下一行  
}  
  
int main(void) {  
    while(cin >> n >> k) {  
        if(n == -1 && k == -1) {  
            break;  
        }  
  
        sum = 0;  
        for(int i = 0;i < n;i++) {  
            for(int j = 0;j < n;j++) {  
                cin >> vis[i][j];  
            }  
        }  
        for(int i = 0;i < n;i++) {  
            isSafe[i] = 1;  
        }  
        dfs(0,0);  
        cout << sum << endl;  
    }  
      
    return 0;  
}  







    
    


