/*************************************************************************
	> File Name: 2.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 08时07分30秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main(){
    int n, a, b;
    cin >> n;

    while(n--){
        cin >> a >> b;
        cout << (a % 100 + b % 100) % 100 << endl; 
    }

    return 0;
}
