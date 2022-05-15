#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

#include "Noncopyable.h"

#include <memory>
#include <functional>

using std::shared_ptr;

class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

class TcpConnection
:Noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection();

    string receive();
    void send(const string &msg);
    void sendInLoop(const string &msg);

    void setAllCallbakcs(const TcpConnectionCallback &conCb, 
                        const TcpConnectionCallback &msgCb,
                        const TcpConnectionCallback &clsCb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    bool isClosed() const;

    string showConnection() const;

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    EventLoop *_loop;
    bool _shutdownWrite;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onclose;
};

#endif
