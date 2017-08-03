/*************************************************************************
    libportal, A Network Library
    Copyright (C) 2017  hepangda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/
#include<portal/socket.h>
#include<portal/global.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
#include<cstring>
#include<errno.h>
#include<fcntl.h>

using libportal::Net;
using libportal::TCPSocket;
using libportal::TCPClient;

std::string Net::GetServerIP(std::string server) {
    struct hostent *ht;
    ht = gethostbyname(server.c_str());
    if (ht == NULL) {
        libportal::lib_error("");
        return "";
    }

    std::string ret = inet_ntoa(*((struct in_addr *)ht->h_addr));
    return ret;
}

TCPSocket::TCPSocket(std::string address, unsigned int port) {
    static const int addr_mode = ADDR_IPV4;

    socket_fd = socket(addr_mode, CONN_TCP, 0);
    if (socket_fd < 0) {
        libportal::lib_error("");
        perror("socket");
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = ADDR_IPV4;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
        perror("pton");
        libportal::lib_error("");
    }
}

TCPSocket::~TCPSocket() {
    close(socket_fd);
}

int TCPSocket::Connect() {
    int ret = connect(socket_fd, (sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        libportal::lib_error("");
        perror("conn err");
    }
    return ret;
}

int TCPSocket::Listen() {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(socket_fd, (sockaddr *)&addr, sizeof(sockaddr_in));
    perror("bind");
    ret = listen(socket_fd, 10);        //TODO:Fix Magic Number
    perror("listen");
    return ret;
}

TCPClient TCPSocket::Accept() {
    auto sz = sizeof(struct sockaddr_in);
    TCPClient ret;
    ret.client_socket = accept(socket_fd, (sockaddr *)&addr, (socklen_t *)&sz);
    return ret;
}

int TCPSocket::Read(std::string &dat) {
    char buf;        //TODO:FIX MAGIC NUMBER
    int ret;
    while ((ret = read(socket_fd, &buf, 1)) > 0) {
        if (buf == 1)
            break;
        dat += buf;
    }
    return ret;
}

int TCPSocket::Write(std::string dat) {
    std::string buf = dat + char(1);
    return write(socket_fd, buf.c_str(), buf.length());
}


int TCPClient::Read(std::string &dat) {
    char buf;        //TODO:FIX MAGIC NUMBER
    int ret;
    while ((ret = read(client_socket, &buf, 1)) > 0) {
        if (buf == 1)
            break;
        dat += buf;
    }
    return ret;
}

int TCPClient::Write(std::string dat) {
    std::string buf = dat + char(1);
    return write(client_socket, buf.c_str(), buf.length());
}

int TCPClient::Close() {
    return close(client_socket);
}