#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Thread.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/Threadpool.h"

#include <unistd.h>

Threadpool::Threadpool(int threadNum, int capacity, DataBag *dataPtr)
:_threadNum(threadNum)
,_capacity(capacity)
,_tq(_capacity)
,_isExit(false)
,_dataPtr(dataPtr)
{
    _threads.reserve(_threadNum);
}

Threadpool::~Threadpool(){
    if(!_isExit){
        stop();
    }
}

void Threadpool::addTask(Task &&task){
    if(task){
        _tq.push(std::move(task));
    }
}

void Threadpool::start(){
    for(int i = 0; i < _threadNum; ++i){
        using namespace std::placeholders;
        DataBag data;
        data = *_dataPtr;
        data.ThreadIdx = i;
        unique_ptr<Thread> th(new Thread(std::bind(&Threadpool::wokerDoTask, this, _1), &data));
        _threads.push_back(std::move(th));
    }

    for(int i = 0; i < _threads.size(); ++i){
        _threads[i]->start();
    }
}

Task Threadpool::getTask(){
    return _tq.pop();
}

void Threadpool::stop(){
    while(!_isExit){
        while(!_tq.isEmpty()){
            sleep(1);
        }

        _isExit = true;
        _tq.wakeup();

        for(int i = 0; i < _threads.size(); ++i){
            _threads[i]->join();
        }
    }
}

void Threadpool::wokerDoTask(DataBag *dataPtr){
    while(!_isExit){
        Task task = getTask();
        if(task){
            task(dataPtr);
        }
    }
}

