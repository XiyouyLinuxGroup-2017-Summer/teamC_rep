/*************************************************************************
	> File Name: 1.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 08时01分54秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main(){
    int n, i;
    char num[12];

    cin >> n;
    while(n--){
        cin >> num;
        cout << 6;
        for(i = 6; i < 11; i++){
            cout << num[i];
        }
        cout << endl;
    }

    return 0;
}
