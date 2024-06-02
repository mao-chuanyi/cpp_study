#include <iostream>
using std::cout;
using std::endl;

class Base{
public:
    Base(int a):
        _a(a)
    {}
    friend Base operator+(const Base & lrh, const Base & rrh);
    friend bool operator==(const Base & rb, int b);
private:
    int _a;
};

Base operator+(const Base & lrh, const Base & rrh)//这里一开始的想法是返回int，但是如果返回int就不满足与内置类型一致性的要求
{
    return Base(rrh._a - lrh._a);
}

bool operator==(const Base & rb, int b)
{
    return rb._a == b;
}

void test(){
    int i = 2;
    int j = 7;
    Base x(i);
    Base y(j);

    cout << (x+y == j - i) << endl;
}

int main(){
    test();
    return 0;
}

