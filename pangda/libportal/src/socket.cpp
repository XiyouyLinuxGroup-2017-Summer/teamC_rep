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
#include<cstring>
#include<errno.h>
using libportal::TCPSocket;
using libportal::TCPClient;

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
    if (connect(socket_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        libportal::lib_error("");
        perror("conn err");
        return -1;
    }
    return 0;
}

int TCPSocket::Listen() {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(socket_fd, (sockaddr *)&addr, sizeof(sockaddr_in));
    perror("bind");
    listen(socket_fd, 10);        //TODO:Fix Magic Number
    perror("listen");
}

TCPClient TCPSocket::Accept() {
    auto sz = sizeof(struct sockaddr_in);
    TCPClient ret;
    ret.client_socket = accept(socket_fd, (sockaddr *)&addr, (socklen_t *)&sz);
    return ret;
}

int TCPSocket::Read(std::string &dat) {
    char buf[51] = { 0 };         //TODO:FIX MAGIC NUMBER
    while (read(socket_fd, buf, 50) > 0) {
        dat += buf;
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}

int TCPSocket::Write(std::string dat) {
    //write(socket_fd, dat.c_str(), dat.length());
    send(socket_fd, dat.c_str(), dat.length(), 0);
}


int TCPClient::Read(std::string &dat) {
    char buf[51] = { 0 };        //TODO:FIX MAGIC NUMBER
    while (read(client_socket, buf, 50) > 0) {
        dat += buf;
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}

int TCPClient::Write(std::string dat) {
    write(client_socket, dat.c_str(), dat.length());
}

int TCPClient::Close() {
    close(client_socket);
}