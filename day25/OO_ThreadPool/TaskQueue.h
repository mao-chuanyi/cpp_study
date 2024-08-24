#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

class Task;
using std::queue;
using ElemType = Task *;
class TaskQueue
{
public:
    TaskQueue(size_t queueSize);
    ~TaskQueue();
    void push(const ElemType & value);
    ElemType pop();
    bool full();
    bool empty();
    void  weakUp();
private:
    size_t _queueSize;
    queue<ElemType> _queue;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _isNotifyAll;    
};

#endif


