/*************************************************************************
	> File Name: test.cpp
	> Author: YinJianxiang
	> Mail: YinJianxiang123@gmail.com
	> Created Time: 2017年07月22日 星期六 08时22分09秒
 ************************************************************************/

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int n;

    cin >> n;
    
    while(n--) {
        string s;
        cin >> s;
        s = s.substr(s.length()-5);
        cout << "6" + s << endl;
    }
    
    return 0;
}

