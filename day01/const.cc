#include <iostream>
using std::cout;
using std::endl;
//#define MAX "100";
#define MAX 100;

void test(){
    const int max = 100;
    cout << max << endl;
}

int main(){
    test();
    return 0;
}

