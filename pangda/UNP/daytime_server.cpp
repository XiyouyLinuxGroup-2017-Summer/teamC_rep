#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<cstring>
#include<arpa/inet.h>
#include<ctime>

int main(int argc, char **argv) {
    int listenfd, connfd;
    sockaddr_in servaddr;
    char buff[500];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13134);

    bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 6);
    

    while (true) {
        connfd = accept(listenfd, (sockaddr *)NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }
}