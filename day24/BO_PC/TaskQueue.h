#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

using std::queue;
class TaskQueue
{
public:
    TaskQueue(size_t queueSize);
    ~TaskQueue();
    void push(const int & value);
    int pop();
    bool full();
    bool empty();

private:
    size_t _queueSize;
    queue<int> _queue;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
        
};

#endif


