#include "MyTask1.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using std::cout;
using std::endl;


MyTask1::MyTask1()
{

}
MyTask1::~MyTask1()
{

}
void MyTask1::process()
{
   ::srand(::clock()); 
   int num = ::rand() % 100;
   cout << ">>MyTask number = " << num << endl;
}
