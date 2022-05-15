#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <sys/epoll.h>

#include "MutexLock.h"

#include <vector>
#include <map>
#include <functional>
#include <memory>

using std::vector;
using std::map;

class Acceptor;

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using Functor = std::function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor &Acceptor);
    ~EventLoop();

    void loop();
    void unloop();
    void runInLoop(Functor &&cb);

    void setAllCallback(TcpConnectionCallback &&conCb,
                        TcpConnectionCallback &&msgCb,
                        TcpConnectionCallback &&clsCb);

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);

    int createEpollFd();
    void addEpollReadFd(int fd);
    void deleteEpollReadFd(int fd);

    int createEventfd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();

private:
    int _efd;
    int _eventfd;
    Acceptor &_acceptor;
    bool _islooping;
    vector<epoll_event> _eventList;
    map<int, TcpConnectionPtr> _conns;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

    vector<Functor> _pendingFunctors;
    MutexLock _mutex;

};

#endif