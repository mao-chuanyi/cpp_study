#ifndef __COMSUMER_H__
#define __COMSUMER_H__
class TaskQueue;
class Comsumer
{
public:
    Comsumer();
    ~Comsumer();

    void comsume(TaskQueue & taskque);
private:

};


#endif

