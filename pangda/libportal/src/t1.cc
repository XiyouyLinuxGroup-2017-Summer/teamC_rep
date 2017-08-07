#include<portal/crypt.h>
#include<portal/network.h>
#include<iostream>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    TCPSocket sock("127.0.0.1", 14001);

    sock.Connect();
    string a;
    cin >> a;
    sleep(1);
    for (int j = 0; j < 8; j++) {
        sock.Write(a);
        string res1;
        sock.Read(res1);
        cout << "[Echo Sever]" << res1 << endl;
        sleep(1);
    }

    return 0;
}