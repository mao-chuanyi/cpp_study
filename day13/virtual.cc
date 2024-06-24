#include <iostream>
using namespace std;
class A
{
public:
    virtual void print() = 0;
    //virtual void display() = 0;
};

class B
: public A
{
public:
    virtual void print() override{
        cout << "B::print()" << endl;
    }
};

int main()
{
    //A a;
    B b;
    b.print();
    A *a = &b;
    a->print();
}
