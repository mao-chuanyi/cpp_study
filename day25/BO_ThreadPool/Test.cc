#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

using std::bind;
using std::cout;
using std::endl;

class MyTask
{
public:
    void process()
    {
        ::srand(::clock());
        int number = ::rand() % 100;
        cout << ">> MyTask number = " << number << endl;
    }
};

void test()
{
    unique_ptr<MyTask> task(new MyTask());
    ThreadPool tp(4, 10);
    tp.start();
    int cnt = 20;
    while (cnt--)
    {
        tp.addTask(bind(&MyTask::process, task.get()));
        cout << "cnt = " << cnt << endl;
    }
    tp.stop();
}

int main()
{
    test();
    return 0;
}
