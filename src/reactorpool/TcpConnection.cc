#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TcpConnection.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/EventLoop.h"

#include <string.h>
#include <sstream>

TcpConnection::TcpConnection(int fd, EventLoop *loop)
    : _sock(fd),
      _sockIO(fd),
      _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()),
      _loop(loop),
      _shutdownWrite(false) {}

TcpConnection::~TcpConnection() {}

InetAddress TcpConnection::getLocalAddr() {
    sockaddr_in localAddr;
    socklen_t len = sizeof(localAddr);
    memset(&localAddr, 0, len);
    if (getsockname(_sock.getFd(), (sockaddr*)&localAddr, &len) < 0) {
        perror("getsockname");
    }
    return InetAddress(localAddr);
}

InetAddress TcpConnection::getPeerAddr() {
    sockaddr_in peerAddr;
    socklen_t len = sizeof(peerAddr);
    memset(&peerAddr, 0, len);
    if (getpeername(_sock.getFd(), (sockaddr*)&peerAddr, &len) < 0) {
        perror("getpeername");
    }
    return InetAddress(peerAddr);
}

void TcpConnection::setAllCallbakcs(const TcpConnectionCallback& conCb,
                                    const TcpConnectionCallback& msgCb,
                                    const TcpConnectionCallback& clsCb) {
    _onConnection = conCb;
    _onMessage = msgCb;
    _onclose = clsCb;
}

string TcpConnection::receive(){
    printf("TcpConnection::receive()\n");
    char buf[65536] = {0};
    int ret = _sockIO.readline(buf, sizeof(buf));
    printf("TcpConnection::receive() return\n");
    return string(buf, ret);
}

void TcpConnection::send(const string &msg){
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

bool TcpConnection::isClosed() const{
    int ret = -1;
    char *buf[64] = {0};
    do{
        ret = recv(_sock.getFd(), buf, sizeof(buf), MSG_PEEK);
    }while(ret == -1 && errno == EINTR);

    return ret == 0;
}

void TcpConnection::handleConnectionCallback(){
    if(_onConnection){
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback(){
    if(_onclose){
        _onclose(shared_from_this());
    }
}

string TcpConnection::showConnection() const{
    std::ostringstream oss;
    oss << _localAddr.getIp() << ":" << _localAddr.getPort() 
        << "--" << _peerAddr.getIp() << ":" << _peerAddr.getPort();
    return oss.str();
}