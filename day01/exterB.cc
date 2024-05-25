#include <iostream>
using std::cout;
using std::endl;

extern int num;
extern void print();

void test01()
{
    cout << num << endl;
    print();
}

int main()
{

    test01();
    return 0;
}
