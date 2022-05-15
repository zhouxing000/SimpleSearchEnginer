#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TcpServer.h"

TcpServer::TcpServer(unsigned short port, const string &ip)
:_acceptor(port, ip)
,_loop(_acceptor){}

TcpServer::~TcpServer(){}

void TcpServer::setAllCallbakcs(TcpConnectionCallback &&conCb, 
                        TcpConnectionCallback &&msgCb,
                        TcpConnectionCallback &&clsCb)
{
    _loop.setAllCallback(std::move(conCb), std::move(msgCb), std::move(clsCb));
}

void TcpServer::start(){
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop(){
    _loop.unloop();
}