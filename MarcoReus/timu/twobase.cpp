/*************************************************************************
	> File Name: twobase.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月27日 星期四 08时06分05秒
 ************************************************************************/

#include <iostream>

using namespace std;

typedef long long ll;

int main(int argc, char *argv[]) {
    ll a;
    ll b;
    cin >> a >> b;
    ll sum_1 = 0;
    for(ll i = 0;i < a;i++) {
        ll t;
        cin >> t;
        sum_1 = sum_1 * b + t;
    }
    ll c;
    ll d;
    cin >> c >> d;
    ll sum_2 = 0;
    for(ll i = 0;i < c;i++) {
        ll t;
        cin >> t;
        sum_2 = sum_2 * d + t;
    }

    if(sum_1 > sum_2) {
        cout << ">" << endl;
    } else if(sum_1 < sum_2) {
        cout << "<" << endl;
    } else {
        cout << "=" << endl;
    }

    return 0;
}

