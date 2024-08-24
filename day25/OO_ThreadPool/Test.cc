#include "ThreadPool.h"
#include "MyTask1.h"
#include <iostream>
using std::cout;
using std::endl;

void test(){
    unique_ptr<Task> ptask(new MyTask1());
    ThreadPool tp(4, 10);
    tp.start();
    int cnt = 20;
    while(cnt--)
    {
        tp.addTask(ptask.get());
        cout << "cnt = " << cnt << endl;
    }
    tp.stop();
}

int main(){
    test();
    return 0;
}

