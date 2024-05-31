#include <iostream>
using std::cout;
using std::endl;

void test(){
    char * str = nullptr;
    cout << "str = " << str << endl;
    cout << "test" << endl;
}

int main(){
    test();
    return 0;
}

