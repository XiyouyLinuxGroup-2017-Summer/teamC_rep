/*************************************************************************
	> File Name: 1.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月21日 星期五 12时26分54秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main(){
    int i;
    double sum, a, aver;

    sum = 0;
    for(i = 0; i < 12; i++){
        cin >> a;
        sum += a;
    }

    aver = ((int)((sum / 12 * 100) + 0.5)) / 100.0;
    cout << '$' << aver << endl;

    return 0;
}
