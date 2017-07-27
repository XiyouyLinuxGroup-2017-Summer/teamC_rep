#include <iostream>
using namespace std;
int flag;
void dfs(long long y, int k){

    if(flag || y < 0)
        return ;
    if(y % k == 0){
        flag = 1;
        cout << y << endl;
        return ;
    }
    cout << "y " << y << endl;
    dfs(y * 10, k);
    dfs(y * 10 +1, k);
}

int main(void)
{
    int n;
    while(cin >> n && n){
        flag = 0;
        dfs(1, n);
    }
    
    return 0;
}
