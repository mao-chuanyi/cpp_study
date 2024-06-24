#include <iostream>
#include <math.h>
using std::cout;
using std::endl;

class Circle
{
public:
    Circle()
    {
        _r = 0;
    }
    Circle(double r):
        _r(r)
    {}

    double getArea()
    {
        return M_PI*_r*_r;
    }
    
    double getPerimeter()
    {
        return 2*M_PI*_r;
    }

    void show()
    {
        cout << _r << endl;
        cout << this->getPerimeter() << endl;
        cout << this->getArea() << endl;
    }
private:
    double _r;
};

class Cylinder
:public Circle
{
public:
    Cylinder(double r, double h):
        Circle(r),
        _h(h)
    {}
    double getVolume()
    {
        return this->getArea()*_h;
    }
    void showVolume()
    {
        cout << this->getVolume() << endl;
    }
private:
    double _h;
};

void test(){
    Cylinder cl(4,5);
    cl.showVolume();
}

int main(){
    test();
    return 0;
}

