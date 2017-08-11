/*************************************************************************
	> File Name: chufa.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月11日 星期五 08时19分28秒
 ************************************************************************/

#include <iostream>
#include <cstdio>  
#include <cstring>

using namespace std;

int number[15];  
bool check(int a, int b) {  
    if (a > 98765) { 
		return false;
	}    
    memset(number,0,sizeof(number));  
    if (b < 10000) { 
		number[0] = 1;
	}    
    while (a) {    
        number[a % 10] = 1;    
        a /= 10;    
    }    
    while ( b ) {    
        number[b % 10] = 1;    
        b /= 10;    
    }    
    int sum = 0;    
    for (int i = 0; i < 10; i++)    
        sum += number[i];    
    return sum == 10;    
}  
int main() {  
    int n;
	int cnt = 0;  
    while (cin >> n && n) {  
        if (cnt++)  {
			cout << "\n";
		} 
        bool flag = false;  
        for (int i = 1234; i < 99999; i++) {  
            if (check(i * n, i) == true) {  
                printf("%05d / %05d = %d\n", i * n, i, n);  
                flag = 1;  
            }  
        }  
        if (flag == false) {  
            printf("There are no solutions for %d.\n",n);  
        }  
    }  
    return 0;  
}  