#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Condition.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/MutexLock.h"


#include <stdio.h>
#include <errno.h>

Condition::Condition(MutexLock &mutex)
:_mutex(mutex)
{
    int ret;
    if(0 != (ret = pthread_cond_init(&_cond, nullptr))){
        errno = ret;
        perror("pthread_cond_init");
    }
}

Condition::~Condition(){
    int ret;
    if(0 != (ret = pthread_cond_destroy(&_cond))){
        errno = ret;
        perror("pthread_cond_destroy");
    }
}

void Condition::notify(){
    int ret;
    if(0 != (ret = pthread_cond_signal(&_cond))){
        errno = ret;
        perror("pthread_cond_signal");
    }
}

void Condition::notifyAll(){
    int ret;
    if(0 != (ret = pthread_cond_broadcast(&_cond))){
        errno = ret;
        perror("pthread_cond_broadcast");
    }
}

void Condition::wait(){
    int ret;
    if(0 != (ret = pthread_cond_wait(&_cond, _mutex.getMutex()))){
        errno = ret;
        perror("pthread_cond_broadcast");
    }
}