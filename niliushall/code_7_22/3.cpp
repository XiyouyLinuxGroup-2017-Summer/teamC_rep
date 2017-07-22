/*************************************************************************
	> File Name: 3.cpp
	> Author: wangliang
    > Email: sdwllinux@gmail.com
	> Created Time: 2017年07月22日 星期六 08时14分26秒
 ************************************************************************/

#include<iostream>
using namespace std;
int main(){
    int c, v0, v1, a, l;
    int sum = 0, count = 0;
    while(cin >> c >> v0 >> v1 >> a >> l){
        v0 -= a;
        sum = 0;
        count = 0;
        while(sum < c){
            v0 += a;
            if(v0 > v1){
                v0 = v1;
            }
            if(!count)
                sum += v0;
            else
                sum += (v0 - l);
            count++;
        }
        cout << count << endl;
    }
    return 0;
}
