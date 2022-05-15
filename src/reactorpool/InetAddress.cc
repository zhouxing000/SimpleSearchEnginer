#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/InetAddress.h"

#include <string.h>
#include <arpa/inet.h>


InetAddress::InetAddress(unsigned short port, const string &ip){
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const sockaddr_in &addr):_addr(addr){}

InetAddress::~InetAddress(){}

string InetAddress::getIp() const{
    return inet_ntoa(_addr.sin_addr);
}

unsigned short InetAddress::getPort() const{
    return ntohs(_addr.sin_port);
}

sockaddr_in *InetAddress::getSockaddrprt(){
    return &_addr;
}