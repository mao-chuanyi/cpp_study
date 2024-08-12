#include "Thread.h"
#include "Producer.h"
#include "Comsumer.h"
#include "TaskQueue.h"
#include <iostream>
#include <memory>
#include <functional>
using std::cout;
using std::endl;
using std::bind;
using std::unique_ptr;
void test(){
    TaskQueue taskque(10);//创建一个容量为10的仓库

    Producer producer;
    Comsumer comsumer;

    unique_ptr<Thread> pro(new Thread(bind(&Producer::produce, &producer, std::ref(taskque))));
    unique_ptr<Thread> com(new Thread(bind(&Comsumer::comsume, &comsumer, std::ref(taskque))));

    pro->start();
    com->start();

    pro->join();
    com->join();
}

int main(){
    test();
    return 0;
}

