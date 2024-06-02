#include <iostream>
#include <unistd.h>
using std::cout;
using std::endl;

void test1(){
    for(int i = 0; i < 106; ++i)
    {
        cout << 'a' ;
    }
        sleep(2);
}
int main(){
    test1();
    return 0;
}

