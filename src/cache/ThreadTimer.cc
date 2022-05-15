#include "/home/mikezhou/codes/SearchEnginerend/include/cache/ThreadTimer.h"

#include <stdio.h>
#include <errno.h>

ThreadTimer::ThreadTimer(ThreadCallback &&cb)
:_pthid(0), _isRunning(false),_cb(std::move(cb)){}

ThreadTimer::~ThreadTimer(){
    if(_isRunning){
        pthread_detach(_pthid);
        _isRunning = false;
    }
}

void ThreadTimer::start(){
    int ret = 0;
    if(0 != (ret = pthread_create(&_pthid, nullptr, threadFunc, this))){
        errno = ret;
        perror("pthread_create");
    }
    _isRunning = true;
}

void ThreadTimer::join(){
    if(_isRunning){
        int ret = 0;
        if(0 != (ret = pthread_join(_pthid, nullptr))){
            errno = ret;
            perror("pthread_create");
        }
        _isRunning = false;
    } 
}

void *ThreadTimer::threadFunc(void *p){
    ThreadTimer *pt = (ThreadTimer *)p;
    if(pt){
        pt->_cb();
    }
    return nullptr;
}
