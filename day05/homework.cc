#include <iostream>
using std::cout;
using std::endl;

class Point{
public:
    static Point * getInstance(){
        Point *p = new Point(0,0);
        return p;
    }
private:
    Point(int x, int y):
        _x(x),
        _y(y)
    {
        cout << "构造" << endl;
    }
    ~Point(){
        cout << "析构" << endl;
    }
private:
    int _x;
    int _y;
};

void test(){
    Point * p1 = Point::getInstance();
}

int main(){
    test();
    return 0;
}

