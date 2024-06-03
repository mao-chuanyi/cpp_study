#include <iostream>
#include <istream>
#include <limits>
using namespace std;

void input(istream & irh, int & x){
    while(irh >> x , !irh.eof()){
        if(irh.bad()){
            cout << "badbit" << endl;
            break;
        }else if(irh.fail()){
            cout << "input a int!" << endl;
            irh.clear();
            irh.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }else if(irh.good()){
            break;
        }
    }
}

class Point{
public:
    Point(int x, int y):
        _x(x),
        _y(y)
    {}
    friend istream & operator>>(istream & irh, Point & p);
    friend ostream & operator<<(ostream & orh, const Point & p);
private:
    int _x;
    int _y;
};


istream & operator>>(istream & irh,  Point & p){
    cout << "input x:" << endl;
    input(irh, p._x);
    cout << "input y:" << endl;
    input(irh, p._y);
    return irh;

}
ostream & operator<<(ostream & orh, const Point & p){
    orh << "(" << p._x << "," << p._y << ")" ;
    return orh;
}

void test(){
    Point p1(1,2);
    cout << p1 << endl;
    cin >> p1 ;
    cout << p1 << endl;
}

int main(){
    test();
    return 0;
}

