#ifndef __MYTASK_H__
#define __MYTASK_H__

#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TcpConnection.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/DataBag.h"



class MyTask
{
public:
    MyTask(const string &, const TcpConnectionPtr &);
    ~MyTask();

    void process(DataBag *dataPtr);
    void parse();

private:
    string _request;
    int operatorId;
    int msgLen;
    string msgContent;
    TcpConnectionPtr _conn;
};

#endif