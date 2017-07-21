/*************************************************************************
	> File Name: 2.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月21日 星期五 12时01分51秒
 ************************************************************************/

#include<iostream>
#include<string.h>
using namespace std;
int main(){
    char str[100];
    int i, n, len;
    cin >> n;
    
    while(n--){
        scanf("%s", str);
        len = strlen(str);
        for(i = 0; (i < len / 2) && (str[i] == str[len - i - 1]); i++)
            ;
        if(i >= len / 2){
            cout << "yes" << endl;
        }
        else{
            cout << "no" << endl; 
        }
    }

    return 0;
}

