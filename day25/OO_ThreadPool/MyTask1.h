#ifndef __MYTASK_H__
#define __MYTASK_H__
#include "Task.h"

class MyTask1
:public Task
{
public:
    MyTask1();
    ~MyTask1();
    void process() override;

};

#endif
