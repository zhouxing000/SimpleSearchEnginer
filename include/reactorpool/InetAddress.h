#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>

#include <string>

using std::string;

class InetAddress
{
public:
    InetAddress(unsigned short port, const string &ip);
    InetAddress(const sockaddr_in &addr);
    ~InetAddress();

    unsigned short getPort() const;
    string getIp() const;

    sockaddr_in *getSockaddrprt();

private:
    sockaddr_in _addr;
};

#endif