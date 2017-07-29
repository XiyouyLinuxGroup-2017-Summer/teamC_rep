#include<portal/crypt.h>
#include<portal/socket.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    TCPSocket sock("127.0.0.1", 13135);
    sock.Listen();
    int t;
    while ((t = sock.Accept())) {
        printf("accepted!\n");
        write(t, "Hello", 6);
        close(t);
    }
    return 0;
}