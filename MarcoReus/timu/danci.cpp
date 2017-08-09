/*************************************************************************
	> File Name: danci.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月08日 星期二 17时02分49秒
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cctype>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>

using namespace std;

string s;
char word[5005*200][201];

map<string, int> map_1;

int main() {
    int j = 0;
    int t;
    while(getline(cin,s)) {
        word[j][t] = 0;
        j++;
        t = 0;
        int len = s.length();
        for(int i = 0;i < len;i++) {
            if(isalpha(s[i])) {
                s[i] = tolower(s[i]);
                word[j][t++] = s[i];
            } else {
                word[j][t] = 0;
                j++;
                t = 0;
            }
        }
        
    }
    
    for(int i = 1;i <= j;i++) {
        if(strlen(word[i]) == 0) {
            continue;
        }
        string t  = word[i];
        map_1.insert(map<string,int>::value_type(t,i));    
    }
    map<string,int>::iterator it;
    
    for(it = map_1.begin();it != map_1.end();it++) {
        cout << it->first << endl;
    }
    
    
    return 0;
}


























