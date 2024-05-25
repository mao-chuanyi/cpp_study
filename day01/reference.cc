#include <iostream>
using std::cout;
using std::endl;

void test(){
    //指针
    int a = 10;
    int *pa = &a;
    cout << "&a = " << &a << endl;
    cout << "pa = " << pa << endl;
    cout << "&pa = " << &pa << endl;
    //引用
    int b = 20;
    int & rb = b;
    cout << "&b = " << &b << endl;
    cout << "&rb = " << &rb << endl;
    /* cout << "&&b = " << &&b << endl; */
    /* cout << "&&rb = " << &&rb << endl; */
    
}

int main(){
    test();
    return 0;
}

