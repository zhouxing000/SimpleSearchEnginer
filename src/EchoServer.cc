#include "/home/mikezhou/codes/SearchEnginerend/include/EchoServer.h"

#include <iostream>

using std::cout;
using std::endl;

EchoServer::EchoServer(DataBag *dataPtr, unsigned short port, const string &ip)
:_dataPtr(dataPtr)
,_threadpool(4, 1000, dataPtr)
,_server(port, ip){
    using namespace std::placeholders;
    _server.setAllCallbakcs(std::bind(&EchoServer::onConnection, this, _1),
                            std::bind(&EchoServer::onMessage, this, _1),
                            std::bind(&EchoServer::onClose, this, _1));
}

EchoServer::~EchoServer(){}

void EchoServer::onConnection(const TcpConnectionPtr &conn){
    cout << conn->showConnection() << "connected" <<endl;
}

void EchoServer::onMessage(const TcpConnectionPtr &conn){
    string msg = conn->receive();
    cout << "recevie from client:" << msg << endl;
    cout << "msgSz:" << msg.size() << endl;
    MyTask mytask(msg, conn);
    using namespace std::placeholders;
    _threadpool.addTask(std::bind(&MyTask::process, mytask, _1));
}

void EchoServer::onClose(const TcpConnectionPtr &conn){
    cout << conn->showConnection() << "closed" << endl;
}

void EchoServer::start(){
    _threadpool.start();
    _server.start();
}
