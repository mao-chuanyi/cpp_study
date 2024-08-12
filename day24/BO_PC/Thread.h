#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
class Thread
{
    using ThreadCallback = std::function<void()>;
public:
    Thread(ThreadCallback && cb);
    ~Thread();
    void start();
    void join();

private:
    static void * threadFunc(void * org);
private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};


#endif

