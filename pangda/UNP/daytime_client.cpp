#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<cstring>
#include<arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd, n;
    char recvline[500];
    struct sockaddr_in servaddr;

    if (argc != 2) {
        fprintf(stderr, "usage: tp <IPAddress>");
        exit(-1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fd");
        exit(-1);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13134);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "Not such address.");
        exit(-1);
    }

    if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(-1);
    }

    while ((n = read(sockfd, recvline, 500)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            perror("fputs");
            exit(-1);
        }
    }
    
    if (n < 0) {
        perror("read");
        exit(-1);
    }

    exit(0);
}