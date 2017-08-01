#include<portal/crypt.h>
#include<portal/socket.h>
#include<sys/epoll.h>
//#include<portal/multiplexing.h>
#include<iostream>
#include<cstdio>
#include<string>
#include<signal.h>
using namespace std;
using namespace libportal;

int WATCHDOG = 1;
MultiplexEpoll poll;

void onexit(int s) {
    WATCHDOG = 0;
}

void *echo(void *arg) {
    epoll_event evts[100];
    int limit;
    while (limit = poll.Wait(evts, 100)) {
        for (int i = 0; i < limit; i++) {
            TCPClient *clt = (TCPClient *)evts[i].data.ptr;
            //for (int j = 0; j < 100; j++) {
                string recv;
                clt->Read(recv);
                cout << recv << endl;
                clt->Write(recv);
            //}
            //clt->Close();
            //poll.Delete(clt->getfd());
            //delete clt;
        }

    }
}

int main() {
    TCPSocket sock("127.0.0.1", 14001);
    sock.Listen();
    signal(SIGINT, &onexit);

    pthread_t handle;
    pthread_create(&handle, NULL, echo, NULL);
    while (WATCHDOG) {
        TCPClient *clt = new TCPClient;
        *clt = sock.Accept();
        poll.Add(clt->getfd(), EPOLLET | EPOLLIN, clt);
    }
    return 0;
}