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
#include<sys/epoll.h>
#include<unistd.h>
namespace libportal {

class MultiplexEpoll {
private:
    int epoll_fd;
public:
    MultiplexEpoll();
    ~MultiplexEpoll();
    int Add(int fd, unsigned int events, void *ptr);

    int Modify(int fd, unsigned int events, void *ptr);

    int Delete(int fd);

    int Wait(epoll_event *events, int size);
    int WaitUntil(epoll_event *events, int size, int seconds);
};


}