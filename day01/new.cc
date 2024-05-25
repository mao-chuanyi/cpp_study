#include <iostream>
using std::cout;
using std::endl;

void test(){
    int *p = new int(10);
    cout << *p << endl;
    delete p;
}
void test2(){
    int *p2 = new int(11);
    cout << *p2 << endl;
    delete p2;
}
int main(){
    test();
    test2();
    return 0;
}

