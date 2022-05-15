#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/MutexLock.h"

#include <stdio.h>
#include <errno.h>

MutexLock::MutexLock(){
    int ret;
    if(0 != (ret = pthread_mutex_init(&_mutex, nullptr))){
        errno = ret;
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock(){
    int ret;
    if(0 != (ret = pthread_mutex_destroy(&_mutex))){
        errno = ret;
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock(){
    int ret;
    if(0 != (ret = pthread_mutex_lock(&_mutex))){
        errno = ret;
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unlock(){
    int ret;
    if(0 != (ret = pthread_mutex_unlock(&_mutex))){
        errno = ret;
        perror("pthread_mutex_unlock");
    }
}

pthread_mutex_t *MutexLock::getMutex(){
    return &_mutex;
}

MutexLockGuard::MutexLockGuard(MutexLock &mutex)
:_mutex(mutex){
    _mutex.lock();
}

MutexLockGuard::~MutexLockGuard(){
    _mutex.unlock();
}