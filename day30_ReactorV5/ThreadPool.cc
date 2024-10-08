#include "ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queueSize)
    : _threadNum(threadNum), _queueSize(queueSize), _taskQueue(_queueSize), _isExit(false)
{
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{
}

// 进程池的开启
void ThreadPool::start()
{
    // 创建工作线程,并且让它们都运行起来
    while (_threadNum--)
    {
        /* unique_ptr<thread> up(new thread(&ThreadPool::doTask, this)); */
        _threads.push_back(std::make_unique<thread>(&ThreadPool::doTask, this));
    }
}

// 进程池的关闭
void ThreadPool::stop()
{
    // 若主线程执行的很快，就会在任务执行结局前先一步来回收子线程，
    // 就会导致任务无法指向完毕
    while (!_taskQueue.empty())
    {
        sleep(1);
    }
    _isExit = true;

    // 唤醒所有在等待队列中的线程，然后逐一回收
    _taskQueue.weakUp();
    for (auto &th : _threads)
    {
        th->join();
    }
}
// 添加任务和获取任务
void ThreadPool::addTask(Task &&task)
{
    if (task)
    {
        _taskQueue.push(std::move(task));
    }
}
Task ThreadPool::getTask()
{
    return _taskQueue.pop();
}

// 执行任务
void ThreadPool::doTask()
{
    while (!_isExit)
    {
        Task task = getTask();
        if (task)
        {
            task(); // 回调
        }
        else
        {
            cout << "ptask is nullptr" << endl;
        }
    }
}
