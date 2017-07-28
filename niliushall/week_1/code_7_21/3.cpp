/*************************************************************************
	> File Name: 3.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月21日 星期五 11时33分12秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main(){
    int n, m, a;
    cin >> n;

    while(n--){
        cin >> m;
        a = 3;
        while(m--){
            a = (a - 1) * 2;
        }
        cout << a << endl;
    }
}
