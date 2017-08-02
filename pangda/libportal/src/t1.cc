#include<portal/crypt.h>
#include<portal/socket.h>
<<<<<<< HEAD
=======
//#include<portal/multiplexing.h>
>>>>>>> d0706c3cc234a13fa7db8013657873b2e12ba570
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
<<<<<<< HEAD
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
=======
    TCPSocket sock("127.0.0.1", 14001);

    sock.Connect();
    string a;
    cin >> a;
    sleep(1);
    for (int j = 0; j < 100; j++) {
        sock.Write(a);
        string res1;
        sock.Read(res1);
        cout << "[Echo Sever]" << res1 << endl;
        sleep(1);
    }
>>>>>>> d0706c3cc234a13fa7db8013657873b2e12ba570

    return 0;
}