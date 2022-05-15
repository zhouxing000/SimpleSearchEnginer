#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>
#include "Noncopyable.h"

class MutexLock
:public Noncopyable
{public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t *getMutex();
private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &);
    ~MutexLockGuard();

private:
    MutexLock &_mutex;
};

#endif

