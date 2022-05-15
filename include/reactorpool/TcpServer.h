#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"

class TcpServer
{
public:
    TcpServer(unsigned short port, const string &ip = "127.0.0.1");
    ~TcpServer();

    void setAllCallbakcs(TcpConnectionCallback &&conCb, 
                        TcpConnectionCallback &&msgCb,
                        TcpConnectionCallback &&clsCb);
    
    void start();
    void stop();

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif