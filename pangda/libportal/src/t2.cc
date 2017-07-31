#include<portal/crypt.h>
#include<portal/socket.h>
#include<iostream>
#include<cstdio>
#include<string>

using namespace std;
using namespace libportal;
int main() {
    TCPSocket sock("127.0.0.1", 14000);
    sock.Listen();
    TCPClient clt;
    std::string recv;
    clt = sock.Accept();
    printf("accepted!\n");
    clt.Read(recv);
    printf("I've read it!");
    clt.Write(recv);
    clt.Close();
    return 0;
}