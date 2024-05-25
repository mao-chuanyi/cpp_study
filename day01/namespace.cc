#include <iostream>
using std::cout;
using std::endl;

namespace wd{
int num = 10;
void func(){
    
}
}//end of wd


void test(){
    int wd::num = 20;
    wd::func();:
}

int main(){
    test();
    return 0;
}

