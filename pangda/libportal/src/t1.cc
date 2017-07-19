#include<portal/crypt.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    string a = CryptRSA::encrypt("pubkey.pem", "gaygayxiaoyuan");
    cout << "Before Decrypt:" << a << endl << endl;;
    cout << "After Decrypt:" << CryptRSA::decrypt("prikey.pem", a) << endl;
    return 0;
}