#include <iostream>
using std::cout;
using std::endl;

class Point{

    class AutoRelease{
        AutoRelease(){}
        ~AutoRelease(){
            if(_pInstance != nullptr){
                delete _pInstance;
                _pInstance = nullptr;
            }
            cout << "~AutoRelease" << endl;
        }
        friend class Point;//外部类访问内部类的私有成员需要友元
    };

public:
    static Point * getInstance(){
        if(_pInstance == nullptr){
            _pInstance = new Point(1,2);
        }
        return _pInstance;
    }
private:
    Point(int x, int y):
        _x(x),
        _y(y)
    {
        cout << "Point(int,int)" << endl;
    }
    ~Point(){
        cout << "~Point()" << endl;
    };
private:
    int _x;
    int _y;
    static Point * _pInstance;
    static AutoRelease _ar;//这里是外部类访问内部类的私有成员
};

Point * Point::_pInstance = nullptr;
Point::AutoRelease Point::_ar;

void test(){
    Point::getInstance();

}

int main(){
    test();
    return 0;
}

