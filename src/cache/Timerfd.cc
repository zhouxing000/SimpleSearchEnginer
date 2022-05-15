#include "/home/mikezhou/codes/SearchEnginerend/include/cache/Timerfd.h"

#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;


Timerfd::Timerfd(int initTime, int periTime, TimeCallback && cb)
:_initTime(initTime)
,_periTime(periTime)
,_cb(std::move(cb)){
    _timerfd = createTimerfd();
}

Timerfd::~Timerfd(){
    if(_isStarted){
        stop();
    }
}

void Timerfd::start(){
    _isStarted = true;
    setTimerfd(_initTime, _periTime);

    pollfd pfd;
    pfd.fd = _timerfd;
    pfd.events = POLLIN;
    int readyNum = 0;
    while(_isStarted){
        readyNum = poll(&pfd, 1, 6000);
        if(readyNum == -1 && errno == EINTR){
            continue;
        }else if(readyNum == -1){
            perror("poll");
            return;
        }else if(readyNum == 0){
            cout << "timerfd time out" << endl;
        }else{
            if(pfd.events & POLLIN){
                handleRead();

                if(_cb){
                    _cb();
                }
            }
        }
    }
}

void Timerfd::stop(){
    _isStarted = false;
    setTimerfd(0, 0);
}

int Timerfd::createTimerfd(){
    int fd = ::timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0){
        perror("timerfd_create");
    }
    return fd;
}

void Timerfd::setTimerfd(int initTime, int periTime){
    itimerspec val;
    val.it_value.tv_sec = initTime;
    val.it_value.tv_nsec = 0;
    val.it_interval.tv_sec = periTime;
    val.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &val, nullptr);
    if(ret < 0){
        perror("timerfd_settime");
    }
}

void Timerfd::handleRead(){
    uint64_t num = 0;
    int ret = read(_timerfd, &num, sizeof(num));
    if(ret != sizeof(num)){
        perror("read");
    }
}