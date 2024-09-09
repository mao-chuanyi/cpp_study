#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t number)
    : _queueSize(number), _queue(), _mutex(), _notFull(), _notEmpty(), _isNotifyAll(false)
{
}

TaskQueue::~TaskQueue()
{
}

void TaskQueue::push(ElemType &&value)
{
    // 若在解锁之前退出了该函数（作用域）， 会发生死锁
    // 使用RAII思想（栈对象的生命周期管理资源）方式防止死锁
    // 当该函数结束时会自动调用MuLoGua的析构函数从而解锁
    // 上锁
    /* _mutex.lock(); */

    unique_lock<mutex> autolock(_mutex);
    // 存满
    /* if(full()) */
    while (full()) // 解决虚假唤醒问题
    {
        _notFull.wait(autolock); // 当消费者调用_notFull.signal()后会从这行代码至少唤醒一个生产者
                                 // 如果唤醒的是多个生产者的化，使用if就会出错
    }

    _queue.push(std::move(value));
    // 仓库有货后唤醒消费者
    _notEmpty.notify_one();

    // 解锁
    /* _mutex.unlock(); */
}

ElemType TaskQueue::pop()
{
    // 加锁
    /* _mutex.lock(); */

    unique_lock<mutex> autolock(_mutex);
    // 空的
    /* if(empty()) */
    while (empty() && !_isNotifyAll)
    {
        // 进入等待队列
        _notEmpty.wait(autolock);
    }
    if (!_isNotifyAll)
    {
        // 有货物
        ElemType tep = _queue.front();
        _queue.pop();
        _notFull.notify_one();
        return tep;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::weakUp()
{
    _notEmpty.notify_all();//唤醒所有被阻塞的消费者
    _isNotifyAll = true;
}

bool TaskQueue::full()
{
    return _queue.size() == _queueSize;
}

bool TaskQueue::empty()
{
    return _queue.size() == 0;
}
