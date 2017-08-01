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
#include<portal/multiplexing.h>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

namespace libportal {

class Socket {
protected:
    int socket_fd = -1;
    sockaddr_in addr;
    const int ADDR_IPV4 = AF_INET;
    const int CONN_TCP = SOCK_STREAM;
    const int CONN_UDP = SOCK_DGRAM;
public:
    virtual int Connect() = 0;
};   

class TCPClient {
private:
    int client_socket;
public:
    friend class TCPSocket;
    //friend class MultiplexEpoll;
    int getfd() {
        return client_socket;
    }
    int Read(std::string &dat);
    int Write(std::string dat);
    int Close();
}; 


class TCPSocket: public Socket {
private:

public:
    TCPSocket(std::string address, unsigned int port);
    ~TCPSocket();
    int Connect();
    int Listen();
    TCPClient Accept();
    int Write(std::string dat);
    int Read(std::string &dat);

};

class UDPSocket: public Socket {
private:

public:
    UDPSocket(std::string address, unsigned int port);
    ~UDPSocket();

};



}