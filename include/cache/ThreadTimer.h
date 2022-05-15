#ifndef __THREADTIMER_H__
#define __THREADTIMER_H__


#include <pthread.h>
#include <functional>

class ThreadTimer{
public:
    using ThreadCallback = std::function<void()>;
    ThreadTimer(ThreadCallback &&cb);
    ~ThreadTimer();
    void start();
    void join();

private:
    static void* threadFunc(void *);

    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif

