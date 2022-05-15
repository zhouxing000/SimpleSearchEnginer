#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/EventLoop.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Acceptor.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TcpConnection.h"

#include <unistd.h>
#include <sys/eventfd.h>

#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor)
:_efd(createEpollFd())
,_eventfd(createEventfd())
,_acceptor(acceptor)
,_islooping(false)
,_eventList(1024)
{
    addEpollReadFd(_acceptor.getFd());
    addEpollReadFd(_eventfd);
}

EventLoop::~EventLoop(){
    close(_efd);
}

void EventLoop::loop(){
    _islooping = true;
    while(_islooping){
        waitEpollFd();
    }
}

void EventLoop::unloop(){
    _islooping = false;
}

void EventLoop::runInLoop(Functor &&cb){
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
    cout << "wakeup IO thread" << endl;
}

 void EventLoop::setAllCallback(TcpConnectionCallback &&conCb,
                        TcpConnectionCallback &&msgCb,
                        TcpConnectionCallback &&clsCb){
    _onConnection = std::move(conCb);
    _onMessage = std::move(msgCb);
    _onClose = std::move(clsCb);
}

void EventLoop::waitEpollFd(){
    int readyNum = 0;
    do{
        readyNum = epoll_wait(_efd, &*_eventList.begin(), _eventList.size(), 5000);
    }while(readyNum == -1 && errno == EINTR);

    if(readyNum == -1){
        perror("epoll_wait");
        return;
    }else if(readyNum == 0){
        cout << "epoll_wait timeout" << endl;
    }else{
        if(readyNum == _eventList.size()){
            _eventList.resize(2 * _eventList.size());
        }

        for(int i = 0; i < readyNum; ++i){
            int fd = _eventList[i].data.fd;
            if(fd == _acceptor.getFd()){
                handleNewConnection();
            }else if(fd == _eventfd){
                if(_eventList[i].events & EPOLLIN){
                    handleRead();
                    cout << "doPendingFunctors" << endl;
                    doPendingFunctors();
                }
            }else{
                if(_eventList[i].events & EPOLLIN){
                    cout << "handleMessage" << endl;
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection(){
    int peerFd = _acceptor.accept();
    addEpollReadFd(peerFd);
    TcpConnectionPtr conn(new TcpConnection(peerFd, this));
    conn->setAllCallbakcs(_onConnection, _onMessage, _onClose);
    _conns.insert(std::make_pair(peerFd, conn));

    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd){
    auto it = _conns.find(fd);
    if(it != _conns.end()){
        bool isClosed = it->second->isClosed();
        if(isClosed){
            it->second->handleCloseCallback();
            deleteEpollReadFd(fd);
            _conns.erase(it);
        }else{
            it->second->handleMessageCallback();
        }
    }
}


int EventLoop::createEpollFd(){
    int ret = epoll_create1(0);
    if(ret < 0){
        perror("epoll_create1");
    }
    return ret;
}

void EventLoop::addEpollReadFd(int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &event);
    if(ret < 0){
        perror("epoll_ctl:add");
    }
}

void EventLoop::deleteEpollReadFd(int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &event);
    if(ret < 0){
        perror("epoll_ctl:del");
    }
}

int EventLoop::createEventfd(){
    int fd = eventfd(0, 0);
    if(fd < 0){
        perror("eventfd");
    }
    return fd;
}

void EventLoop::wakeup(){
    uint64_t one = 1;
    int ret = write(_eventfd, &one, sizeof(one));
    if(ret != sizeof(one)){
        perror("write");
    }
}

void EventLoop::handleRead(){
    uint64_t num = 0;
    int ret = read(_eventfd, &num, sizeof(num));
    if(ret != sizeof(num)){
        perror("read");
    }
}

void EventLoop::doPendingFunctors(){
    vector<Functor> tmp;
    {
        MutexLockGuard autolock(_mutex);
        tmp.swap(_pendingFunctors);
    }

    for(int i = 0; i < tmp.size(); ++i){
        tmp[i]();
    }
}