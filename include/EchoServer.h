#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TcpServer.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Threadpool.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/cache/MyTask.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/DataBag.h"

class EchoServer
{
public:
    EchoServer(DataBag *dataPtr, unsigned short port, const string &ip = "127.0.0.1");
    ~EchoServer();

    void onConnection(const TcpConnectionPtr &);
    void onMessage(const TcpConnectionPtr &);
    void onClose(const TcpConnectionPtr &);

    void start();

private:
    Threadpool _threadpool;
    TcpServer _server;
    DataBag *_dataPtr;
};

#endif