#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>
#include "MutexLock.h"
#include "Noncopyable.h"

class MutexLock;
class Condition
:public Noncopyable
{
public:
    Condition(MutexLock &);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};

#endif

