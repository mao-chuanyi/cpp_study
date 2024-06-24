#include <iostream>
using std::cout;
using std::endl;

class A{
public:
    virtual void a(){ cout << "A::a()" << endl; } 
    virtual void b(){ cout << "A::b()" << endl; } 
    virtual void c(){ cout << "A::c()" << endl; } 
};

class B{
public:
    virtual void a(){ cout << "B::a()" << endl; } 
    virtual void b(){ cout << "B::b()" << endl; } 
    void c(){ cout << "B::c()" << endl; } 
    void d(){ cout << "B::d()" << endl; } 
};

class C
: public A
, public B
{
public:
    virtual void a(){ cout << "C::a()" << endl; } 
    void c(){ cout << "C::c()" << endl; } 
    //void d(){ cout << "C::d()" << endl; } 
};
void test(){
    C c;
    c.d();
}

int main(){
    test();
    return 0;
}

