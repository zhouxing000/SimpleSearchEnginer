#ifndef __TIMERFD_H__
#define __TIMERFD_H__

#include <functional>

class Timerfd
{
public:
    using TimeCallback = std::function<void()>;
    Timerfd(int, int, TimeCallback &&);
    ~Timerfd();

    void start();
    void stop();

private:
    int createTimerfd();
    void setTimerfd(int, int);
    void handleRead();

private:
    int _timerfd;
    int _initTime;
    int _periTime;
    bool _isStarted;
    TimeCallback _cb;
};

#endif