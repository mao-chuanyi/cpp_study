#ifndef __WORKTHREAD_H__
#define __WORKTHREAD_H__
#include "Thread.h"

class ThreadPool;
class WorkThread1
:public Thread
{ 
public:
    WorkThread1(ThreadPool & pool);
    ~WorkThread1();
    void run() override;

private:
    ThreadPool & _pool;
};
#endif

