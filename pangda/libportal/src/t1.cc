#include<portal/crypt.h>
#include<portal/socket.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    TCPSocket sock("127.0.0.1", 14000);
    sock.Connect();
    string a;
    cin >> a;
    cout << "I've cined!" << endl;
    sock.Write(a);
    cout << "I've writed!" << endl;
    string res;
    sock.Read(res);
    cout << res << endl;
    /*CryptRSA::generate_keyfile("b.pem", "i.pem");
    string a = "nihao, b";
    CryptRSA::encrypt("b.pem", a);
    cout << "before decrypt: " << endl;
    cout << a << endl;

    CryptRSA::decrypt("i.pem", a);
    cout << "After decrypt: " << endl;
    cout << a << endl;*/

    return 0;
}