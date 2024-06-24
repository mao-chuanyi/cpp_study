#include <iostream>
using std::cout;
using std::endl;
class Point;//前置声明
typedef void (Point::*MemberFunction)();

class Point{
public:
    Point(){}

    Point(int x, int y):
        _x(x),
        _y(y)
    {}
    void print(){
        cout << _x << "," << _y << endl;
    }
    void operator()(){
        cout << "hello" << endl;
    }
private:
    int _x;
    int _y;
};

void test(){
    Point p;
    p();//函数对象（仿函数）
    
    void (Point::*pp)() = &Point::print;//必须指明属于哪个类
    Point p1(1,3);//成员函数指针必须通过对象来调用
    (p1.*pp)();

    MemberFunction ppp = &Point::print;
    (p.*ppp)();
}

int main(){
    test();
    return 0;
}

