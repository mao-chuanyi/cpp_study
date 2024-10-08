#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include <stdio.h>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class Thread;

class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queueSize);
    ~ThreadPool();
    //进程池的开启和关闭
    void start();
    void stop();
    //添加任务和获取任务
    void addTask(Task * ptask);
    Task * getTask();
    //执行任务
    void doTask();
private:
    size_t _threadNum;
    size_t _queueSize;
    vector<unique_ptr<Thread>> _threads;    
    TaskQueue _taskQueue;
    bool _isExit;
};

#endif
