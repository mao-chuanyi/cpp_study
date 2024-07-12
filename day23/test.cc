#include <memory>
#include <iostream>
using std::cout;
using std::endl;


void test(){
    std::allocator<int> al;
    cout << al.max_size() << endl;
    int *p = al.allocate(1);
    cout << "*p = " << *p << endl;
    al.construct(p, 10);
    cout << "*p = " << *p << endl;
    al.destroy(p);
    cout << "*p = " << *p << endl;
    al.deallocate(p, 1);
    cout << "*p = " << *p << endl;

}

int main(){
    test();
    return 0;
}

