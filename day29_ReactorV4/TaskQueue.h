#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

using std::condition_variable;
using std::function;
using std::mutex;
using std::queue;
using std::unique_lock;
using ElemType = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t queueSize);
    ~TaskQueue();
    void push(ElemType &&value);
    ElemType pop();
    bool full();
    bool empty();
    void weakUp();

private:
    size_t _queueSize;
    queue<ElemType> _queue;
    mutex _mutex;
    condition_variable _notFull;  // 生产者的条件变量
    condition_variable _notEmpty; // 消费者的条件变量
    bool _isNotifyAll;
};

#endif
