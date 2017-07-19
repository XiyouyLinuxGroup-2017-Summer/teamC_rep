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


libportal::TCPSocket::TCPSocket() {
    static const int addr_mode = ADDR_IPV4;

    socket_fd = socket(addr_mode, CONN_TCP, 0);
    if (socket_fd < 0) {
        libportal::lib_error("");
    }
}

int libportal::TCPSocket::Connect(std::string address, unsigned int port) {
    sockaddr_in addr;
    addr.sin_family = ADDR_IPV4;
    addr.sin_port = htons(port);
    memset(&addr, 0, sizeof(addr));

    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
        libportal::lib_error("");
        return -1;
    }

    if (connect(socket_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        libportal::lib_error("");
        return -1;
    }
    return 0;
}