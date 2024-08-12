#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__
#include "NonCopyable.h"
#include <pthread.h>


class MutexLock
:public NonCopyable//让这个锁无法复制
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    void tryLock();
    pthread_mutex_t* gerMutexLockPtr();

private:
    
    pthread_mutex_t _mutex;

};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock & mutex)
        :_mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unlock();
    }
private:
    MutexLock & _mutex;
};

#endif

