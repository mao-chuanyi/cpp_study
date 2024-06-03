#include <iostream>
using namespace std;

class Point{
public:
    Point(int x, int y):
        _x(x),
        _y(y)
    {}
    friend ostream & operator<<(ostream & orh, const Point & p);
private:
    int _x;
    int _y;
};


ostream & operator<<(ostream & orh, const Point & p){
    orh << "(" << p._x << "," << p._y << ")" ;
    return orh;
}

void test(){
    Point p1(1,2);
    cout << p1 << endl;
}

int main(){
    test();
    return 0;
}

