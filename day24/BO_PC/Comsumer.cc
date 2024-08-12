#include "TaskQueue.h"    
#include "Comsumer.h"
#include <iostream>
#include <unistd.h>
using std::cout;
using std::endl;

Comsumer::Comsumer()
{
}
Comsumer::~Comsumer()
{}
void Comsumer::comsume(TaskQueue & taskque)
{
    int cnt = 20;
    while(cnt--)
    {
        int number = taskque.pop();
        cout << "<< comsume a number = " << number << endl;
        sleep(1);
    }
}












