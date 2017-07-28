/*#include<iostream>
using namespace std;
int main(){
    int x;
    long long y;
    while(cin >> x && x){
        y = 1;
        while(1){
            if(y % x == 0){
                cout << y << endl;
                break;
            } else if(y % 2 == 0){
                y += 1;
            } else {
                y *= 10;
            }
        }
    }

    return 0;
}
*/
#include <iostream>
using namespace std;
int flag;
void dfs(long long y, int k, int num){  
    if(flag || num >= 19)
        return ;
    if(y % k == 0){
        flag = 1;
        cout << y << endl;
        return ;
    }

    dfs(y * 10, k, num+1);
    dfs(y * 10 +1, k, num+1);
}

int main(void)
{
    int n;
    while(cin >> n && n){
        flag = 0;
        dfs(1, n, 0);
    }
    
    return 0;
}
