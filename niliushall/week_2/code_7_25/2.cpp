/*************************************************************************
	> File Name: 2.cpp
	> Author: niliushall
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月25日 星期二 09时42分09秒
 ************************************************************************/

#include<iostream>
#include<string.h>
using namespace std;
int main(){
    int len, n, len1, len2, t = 0;

    cin >> n;
    getchar();
    for(int i = 0; i < n; i++){
        char a[1002]  ={0}, b[1002] = {0};
        cin >> a >> b;
        len1 = strlen(a);
        len2 = strlen(b);
        len = (len1 > len2) ? len1 : len2;
        a[len + 1]  =0;
       
        cout << "Case " << i+1 << ':' << endl;
        cout << a << " + " << b << ' ' << '=' << ' ';

        for(int j = 0; j < len1; j++)
            a[len - j] = a[len1 - 1 - j];
        for(int j = 0; j < len2; j++)
            b[len - j] = b[len2 - 1 - j];

        for(int j = 0; j <= len - len1; j++)
            a[j] = '0';
        for(int j = 0; j <= len - len2; j++)
            b[j] = '0';
        for(int j = len; j >= 0; j--){
            t += a[j] + b[j] - 96;
            a[j] = t % 10 + 48;
            t /= 10;
        }

        if(a[0] == '0')
            cout << a+1 << endl;
        else
            cout << a << endl;

        if(i != n-1)
            cout << endl;
    }

    return 0;
}
