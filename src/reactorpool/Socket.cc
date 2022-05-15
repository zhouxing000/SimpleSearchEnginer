#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Socket.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>


Socket::Socket(){
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0){
        ::perror("socket");
    }
}

Socket::Socket(int fd):_fd(fd){}

Socket::~Socket(){
    ::close(_fd);
}


int Socket::getFd() const{
    return _fd;
}

void Socket::setNonBlock(){
    int flag = fcntl(_fd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(_fd, F_SETFL, flag);
}

void Socket::shutdownWrite(){
    ::shutdown(_fd, SHUT_WR);
}