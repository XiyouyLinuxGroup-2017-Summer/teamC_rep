/*************************************************************************
	> File Name: 1.cpp
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月24日 星期一 08时07分01秒
 ************************************************************************/

#include<iostream>
#include <string.h>
using namespace std;
int main(){
    int count = 0, max = 9999999, len;
    int a[7] = {0};
    char ch[100001];
    cin >> ch;
    len = strlen(ch);
    for(int i= 0; i < len; i++){
        if(ch[i] == 'B')
            a[0]++;
        else if(ch[i] == 'u')
            a[1]++;
        else if(ch[i] == 'l')
            a[2]++;
        else if(ch[i] == 'b')
            a[3]++;
        else if(ch[i] == 'a')
            a[4]++;
        else if(ch[i] == 's')
            a[5]++;
        else if(ch[i] == 'r')
            a[6]++;
    }

    a[4] /= 2;
    a[1] /= 2;

    for(int i = 0; i < 7; i++){
        if(a[i] < max)
            max = a[i];
    }

    cout << max << endl;

    return 0;
}
