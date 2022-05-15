#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Thread.h"

#include <stdio.h>
#include <errno.h>

Thread::Thread(ThreadCallback &&cb, DataBag *dataPtr)
:_pthid(0), _isRunning(false),_cb(std::move(cb)){
    _data = *dataPtr;
}

Thread::~Thread(){
    if(_isRunning){
        pthread_detach(_pthid);
        _isRunning = false;
    }
}

void Thread::start(){
    int ret = 0;
    if(0 != (ret = pthread_create(&_pthid, nullptr, threadFunc, this))){
        errno = ret;
        perror("pthread_create");
    }
    _isRunning = true;
}

void Thread::join(){
    if(_isRunning){
        int ret = 0;
        if(0 != (ret = pthread_join(_pthid, nullptr))){
            errno = ret;
            perror("pthread_create");
        }
        _isRunning = false;
    } 
}

void *Thread::threadFunc(void *p){
    Thread *pt = (Thread *)p;
    if(pt){
        pt->_cb(&(pt->_data));
    }
    return nullptr;
}
