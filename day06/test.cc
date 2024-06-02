#include <iostream>
using std::cout;
using std::endl;
class Point{
public:
    Point(int x  , int y  ):
        _x(x),
        _y(y)
    {
        cout << "构造" << endl;
    }
    void print(){
        cout << _x << ',' << _y << endl;
    }
private:
    int _x = 0;
    int _y = 0;
};
void test(){
    Point p;
    p.print();
}

int main(){
    test();
    return 0;
}

