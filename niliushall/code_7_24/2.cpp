/*************************************************************************
	> File Name: 22.cpp
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月24日 星期一 14时01分02秒
 ************************************************************************/

#include<iostream>
using namespace std;

long long gcd(long long a, long long b){
    long long x;
    if(a < b){
        x = a;
        a = b;
        b = x;
    }
    while(x = a % b){
        a = b;
        b = x;
    }
    return b;
}

int main(){
    int n;
    char ch;
    long long a, b, c, d, x, y, t;

    cin >> n;
    while(n--){
        cin >> a >> ch >> b >> c >> ch >> d;
        
        x = gcd(a, b);
        a /= x;
        b /= x;
        x = gcd(c, d);
        c /= x;
        d /= x;

        x = a / gcd(a, c) * c;
        y = gcd(b, d);

        if(x % y){
            t = gcd(x, y);
            cout << x/t << '/' << y/t << endl;
        }
        else
            cout << x / y << endl;
    }

    return 0;
}

