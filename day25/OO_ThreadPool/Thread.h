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
public:
    Thread();
    virtual ~Thread();
    void start();
    void stop();
private:
    static void * threadFunc(void * org);
    virtual void run() = 0;
private:
    pthread_t _thid;
    bool _isRunning;
};


#endif

