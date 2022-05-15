#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/SocketIO.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>

SocketIO::SocketIO(int fd):_fd(fd){}

SocketIO::~SocketIO(){}

int SocketIO::readn(char *buf, int size){
    char *bufptr = buf;
    int left = size;
    int ret = 0;
    while(left > 0){
        ret = recv(_fd, bufptr, left, 0);
        if(ret == 0){
            break;
        }else if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("recv");
            break;
        }else{
            bufptr += ret;
            left -= ret;
        }
    }
    return size - left;
}

int SocketIO::readline(char *buf, int maxSz){
    int left = maxSz - 1;
    char *bufptr = buf;
    int ret = 0;
    int total = 0;
    while(left > 0){
        ret = recv(_fd, bufptr, left, MSG_PEEK);
        if(ret == 0){
            break;
        }else if(ret == -1 && errno == EINTR){
            continue;
        }else if(ret == -1){
            perror("recv:MSG_PEEK");
            break;
        }else{
            if(ret + total > maxSz - 1){
                total = maxSz - 1 - ret;
            }
            for(int i = 0; i < ret; ++i){
                if(bufptr[i] == '\n'){
                    int sz = i + 1;
                    readn(bufptr, sz);
                    bufptr[sz] = '\0';
                    return total + sz;
                }
            }

            readn(bufptr, ret);
            left -= ret;
            total += ret;
            bufptr += ret;
        }
    }
    buf[maxSz - 1] = '\0';
    return maxSz - 1;
}

int SocketIO::writen(const char *buf, int size){
    int left = size;
    const char *bufptr = buf;
    int ret = 0;
    while(left > 0){
        ret = send(_fd, bufptr, size, 0);
        if(ret == -1){
            perror("send");
        }else{
            left -= ret;
            bufptr += ret;
        }
    }
    return size - left;
}