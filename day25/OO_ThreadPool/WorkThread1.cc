#include "ThreadPool.h"
#include "WorkThread1.h"


WorkThread1::WorkThread1(ThreadPool & pool)
:_pool(pool)
{

}

WorkThread1::~WorkThread1()
{}

void WorkThread1::run()
{
    _pool.doTask();
}
