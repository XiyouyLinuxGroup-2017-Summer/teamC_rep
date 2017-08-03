#include<portal/crypt.h>
#include<portal/network.h>
#include<signal.h>
#include<iostream>
#include<string>

using namespace std;
using namespace libportal;

int WATCHDOG = 1;
MultiplexEpoll poll;

void onexit(int s) {
    WATCHDOG = 0;
}

void *echo(void *arg) {
    MultiplexEpollEvent evts[100];
    int limit;
    while (limit = poll.Wait(evts, 100)) {
        for (int i = 0; i < limit; i++) {
            TCPClient *clt = evts[i].GetClient();
            string recv;
            int ret = clt->Read(recv);
            if (ret == 0) {
                poll.Delete(*clt);
                clt->Close();
                continue;
            }
            cout << recv << endl;
            clt->Write(recv);

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
        poll.Add(*clt, EPOLLET | EPOLLIN | EPOLLOUT);
    }
    return 0;
}