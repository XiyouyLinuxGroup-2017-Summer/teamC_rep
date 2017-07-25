/*************************************************************************
	> File Name: shui.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月20日 星期四 12时12分02秒
 ************************************************************************/

#include<iostream>
using namespace std;

int main (){
    int count, a, b, c, m, n, i;
    while(cin >> m >> n){
        count = 0;
        for(i = m; i <= n; i++){
            a = i / 100;
            b = i / 10 % 10;
            c = i % 10;

            if((a * a * a + b * b * b + c * c * c) == i){
                count++;
                if(count == 1)
                    cout << i;
                else
                    cout << ' ' << i;
            }
        }
        if(!count){
            cout << "no";
        }
        cout << endl;
    }
}
