#include<portal/crypt.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    string a = CryptMD5::encrypt("hello,world");
    cout << a << endl;
    return 0;
}