/*************************************************************************
	> File Name: qiqiu.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月07日 星期一 09时01分35秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int main() {
    int n;
    int a[1005];
    
    while(cin >> n) {
        if(n == 0) {
            break;
        }
        memset(a,0,sizeof(a));
        string s[1005];
        int t = -1;
        int index = -1;
        for(int i = 0;i < n;i++) {
            cin >> s[i];
        }
        for (int i = 0;i < n;i++) {  
            a[i] = 0;  
            for (int j = i;j < n;j++)  {  
                if (s[i] == s[j]) {  
                    a[i]++;  
                }  
            }  
            if (a[i] > t) {  
                t = a[i];  
                index = i;  
            }  
                  
        }  
        cout << s[index] << "\n";
    }
    
    return 0;    
}











