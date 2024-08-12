#include "Thread.h"

Thread::Thread(ThreadCallback && cb)
:_thid(0)
,_isRunning(false)
,_cb(std::move(cb))
{
}

Thread::~Thread() {}

void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join");
            return;
        }
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread * th= static_cast<Thread *>(arg);
    if(th != nullptr)
    {
        th->_cb();
    }
    else
    {
        cout << "th is nullptr" << endl;
    }
    pthread_exit(nullptr);
}

