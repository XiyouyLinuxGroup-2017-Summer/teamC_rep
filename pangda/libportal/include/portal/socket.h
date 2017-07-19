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
#pragma once
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>

namespace libportal {

class Socket {
protected:
    int socket_fd = -1;
    sockaddr_in addr;
    const int ADDR_IPV4 = AF_INET;
    const int CONN_TCP = SOCK_STREAM;
    const int CONN_UDP = SOCK_DGRAM;
public:
    virtual int Connect(std::string address, unsigned int port) = 0;

};    

class TCPSocket: public Socket {
private:

public:
    TCPSocket();
    virtual int Connect(std::string address, unsigned int port);
};


}