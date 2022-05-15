#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include "Timerfd.h"
#include "ThreadTimer.h"

class TimerThread
{
public:
    using TimerCallback = std::function<void()>;

    TimerThread(int initTime, int periTime, TimerCallback &&cb)
    :_timer(initTime, periTime, std::move(cb))
    ,_thread(std::bind(&Timerfd::start, &_timer))
    {}

    void start(){
        _thread.start();
    }

    void stop(){
        _timer.stop();
    }


private:
    Timerfd _timer;
    ThreadTimer _thread;
};

#endif