#include "/home/mikezhou/codes/SearchEnginerend/include/reactorpool/TaskQueue.h"

TaskQueue::TaskQueue(int capacity)
:_capacity(capacity)
,_que()
,_mutex()
,_condFull(_mutex)
,_condEmpty(_mutex)
,_clearFlag(false){}

TaskQueue::~TaskQueue(){}

bool TaskQueue::isFull(){
    return _que.size() == _capacity;
}

bool TaskQueue::isEmpty(){
    return _que.empty();
}

void TaskQueue::push(const ElemType &elem){
    {// _mutex.lock();
    MutexLockGuard autolock(_mutex);
    if(isFull()){
        _condFull.wait();
    }
    _que.push(elem);
    // _mutex.unlock();
    }
    _condEmpty.notify();
}

TaskQueue::ElemType TaskQueue::pop(){
    MutexLockGuard autolock(_mutex);
        // _mutex.lock();
    while(!_clearFlag && isEmpty()){
        _condEmpty.wait();
    }
    if(!_clearFlag){
        ElemType _data = _que.front();
        _que.pop();
        // _mutex.unlock();
        _condFull.notify();
    
        return _data;
    }else{
        return nullptr;
    }
    
}

void TaskQueue::wakeup(){
    _clearFlag = true;
    _condEmpty.notifyAll();
}
