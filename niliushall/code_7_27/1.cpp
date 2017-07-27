/*************************************************************************
	> File Name: 1.cpp
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月27日 星期四 13时51分11秒
 ************************************************************************/

#include<iostream>
using namespace std;

long long fun(int n, int x){
    long long sum = 0;
    int a;
    while(n--){
        cin >> a;
        sum = sum * x + a;
    }
    return sum;
}

int main(){
    int n, x;
    long long sum1, sum2;
    cin >> n >> x;
    sum1 = fun(n, x);
    cin >> n >> x;
    sum2 = fun(n, x);
    
    if(sum1 > sum2)
        cout << '>' << endl;
    else if(sum1 == sum2)
        cout << '=' << endl;
    else
        cout << '<' << endl;

    return 0;
}
