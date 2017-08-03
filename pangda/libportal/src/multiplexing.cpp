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
#include<ctime>

using libportal::MultiplexEpoll;
using libportal::MultiplexEpollEvent;
using libportal::TCPClient;

MultiplexEpoll::MultiplexEpoll() {
    epoll_fd = epoll_create(100);   //TODO:Fix Magic Number
}

MultiplexEpoll::~MultiplexEpoll() {
    close(epoll_fd);                //TODO:Handle Error
}

int MultiplexEpoll::Add(TCPClient &client, unsigned int events) {
    epoll_data evt_data;
    evt_data.ptr = &client;
    epoll_event this_event = { events, evt_data };

    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client.client_socket, &this_event);
}

int MultiplexEpoll::Modify(TCPClient &client, unsigned int events) {
    epoll_data evt_data;
    evt_data.ptr = &client;
    epoll_event this_event = { events, evt_data };

    return epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client.client_socket, &this_event);
}

int MultiplexEpoll::Delete(TCPClient &client) {
    return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client.client_socket, NULL);
}

int MultiplexEpoll::Wait(MultiplexEpollEvent *events, int size) {
    epoll_event *temp = new epoll_event[size];
    int ret = epoll_wait(epoll_fd, temp, size, -1);
    for (int i = 0; i < size; i++) {
        events[i].self = temp[i];
    }
    delete[] temp;
    return ret;
}

int MultiplexEpoll::WaitUntil(MultiplexEpollEvent *events, int size, int seconds) {
    epoll_event *temp = new epoll_event[size];
    int ret = epoll_wait(epoll_fd, temp, size, time(NULL) + seconds);
    for (int i = 0; i < size; i++) {
        events[i].self = temp[i];
    }
    delete[] temp;
    return ret;
}

TCPClient *MultiplexEpollEvent::GetClient() {
    return (TCPClient *)self.data.ptr;
}

bool MultiplexEpollEvent::CheckEvent(unsigned int event) {
    return self.events & event;
}