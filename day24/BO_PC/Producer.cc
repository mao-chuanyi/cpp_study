#include "Producer.h"
#include "TaskQueue.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using std::cout;
using std::endl;

Producer::Producer(){}
Producer::~Producer(){}

void Producer::produce(TaskQueue & taskque)
{
    ::srand(::clock());//生成随机数种子
                       //加上作用域限定符的目的是为了区别类内成员函数和类外的函数
    int cnt = 20;
    while(cnt--)
    {
        int number = ::rand() % 100;//产生随机数
        taskque.push(number);
        cout << ">> produce a number = " << number << endl;
        sleep(1);
    }
}
