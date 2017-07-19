#include<portal/crypt.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    string a = CryptRSA::encrypt("pubkey.pem", "一段中文字符的测试");
    cout << "before decrypt:" << a << endl;
    cout <<  CryptRSA::decrypt("prikey.pem", a) << endl;
    return 0;
}