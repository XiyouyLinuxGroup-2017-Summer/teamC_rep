#include<portal/crypt.h>
#include<portal/socket.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    TCPSocket sock("127.0.0.1", 13135);
    sock.Connect();
    string res = sock.Read();
    cout << res << endl;
    return 0;
}