#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Acceptor.h"

Acceptor::Acceptor(unsigned short port, const string &ip)
:_listensock()
,_addr(port, ip){}

Acceptor::~Acceptor(){}

int Acceptor::getFd() const{
    return _listensock.getFd();
}

void Acceptor::setReuseAddr(){
    int on = 1;
    if(setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
        perror("setReuseAddr");
    }
}

void Acceptor::setReusePort(){
    int on = 1;
    if(setsockopt(getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on)) < 0){
        perror("setReusePort");
    }
}

void Acceptor::bind(){
    if(::bind(getFd(), (sockaddr*)_addr.getSockaddrprt(), sizeof(_addr)) < 0){
        perror("bind");
    }
}

void Acceptor::listen(){
    if(::listen(getFd(), 1024) < 0){
        perror("listen");
    }
}

int Acceptor::accept(){
    int peerFd = ::accept(getFd(), nullptr, nullptr);
    if(peerFd < 0){
        perror("accept");
    }
    return peerFd;
}

void Acceptor::ready(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}