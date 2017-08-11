/*************************************************************************
	> File Name: kuohao.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月10日 星期四 10时14分07秒
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

int main() {  
    int n;  
    cin >> n;  
    getchar();  
    
    while(n--) {  
        stack<char> s;  
        string str;  
        int flag = 0;  
        getline(cin,str);
        for(int i = 0;i < str.size();i++) {  
            if(str[i]=='[' || str[i]=='(') {
                s.push(str[i]);  
            } else if(!s.empty() && s.top() == '(' && str[i] == ')') {
                s.pop();
            } else if(!s.empty() && s.top() == '[' && str[i] == ']')  {
                s.pop();  
            } else {
                flag = 1;
            }
        }  
        if(!flag && !s.size()) {
            cout<<"Yes"<<endl;  
        } else {
            cout<<"No"<<endl;
        }
     }  
     return 0;  
}  
