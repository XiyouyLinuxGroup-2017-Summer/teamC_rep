/*************************************************************************
	> File Name: hdu1062.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月20日 星期四 11时07分49秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>  
#include<stack> 

using namespace std;  

int main(void) {  
    int n;  
    char ch;  
    
    cin >> n;
    getchar();   
    
    while(n--) {  
        stack<char> s;  
        while(true) {  
            ch=getchar();   
            if(ch==' '||ch=='\n') {  
                while(!s.empty()) {  
                    cout << s.top();
                    s.pop();   
                }  
                if(ch=='\n')    
                    break;  
                printf(" ");  
            }  
            else  
                s.push(ch);  
        }  
        printf("\n");  
    }  
    return 0;  
}  
