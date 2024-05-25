#include <iostream>
using std::cout;
using std::endl;

void test(){
    const char * str = "hello";
    cout << str << endl;
    cout << &str << endl;
    printf("%s\n",str);
}

int main(){
    test();
    return 0;
}

