#include "Thread.h"

Thread::Thread()
:_thid(0)
,_isRunning(false)
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

void Thread::stop()
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
        th->run();
    }
    else
    {
        cout << "th is nullptr" << endl;
    }
    pthread_exit(nullptr);
}

