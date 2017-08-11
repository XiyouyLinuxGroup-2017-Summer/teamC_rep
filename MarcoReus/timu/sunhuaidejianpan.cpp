/*************************************************************************
	> File Name: sunhuaidejianpan.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年08月09日 星期三 08时36分15秒
 ************************************************************************/

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <stack>
#include <list>
#include <queue>
#include <map>
#include <set>

using namespace std;

typedef long long ll;

list<char> link;

int main(int argc,char *argv[]) {
	ios_base::sync_with_stdio(false);
	char s[100001];
	
	while(scanf("%s",s) != EOF) {
	   link.clear();
	   list<char>::iterator it = link.begin();
	   int len = strlen(s);
	   for(int i = 0;i < len;i++) {
	       if(s[i] == '[') {
	           it = link.begin();
	           continue;
	       } else if(s[i] == ']'){
	           it = link.end();
	           continue;
	       }
	       link.insert(it,s[i]);
	   }
	   for(it = link.begin();it != link.end();it++) {
	       cout << *it;
	   }
	   cout << "\n";
	}
	
	return 0;
}
