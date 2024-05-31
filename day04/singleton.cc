#include <iostream>
using std::cout;
using std::endl;

class Point{

    Point(int x, int y):
        _x(x),
        _y(y)
    {
        cout << "构造" <<  endl;
    }

//    Point(const Point & p)://单例模式中要避免一切复制
//        _x(p._x),
//        _y(p._y)
//    {
//        cout << "拷贝" << endl;
//    }
//
//    Point & operator=(const Point & p){
//        _x = p._x;
//        _y = p._y;
//        return *this;
//    }


//c++11中可以禁用默认的拷贝构造和赋值运算符函数
    Point(const Point & p) = delete;
    Point & operator=(const Point & p) = delete;

public:
    static Point & getInstance(){
        static Point p(1,2);//这里的静态局部变量只会被初始化一次
        return p;
    }
    
    
    ~Point(){
        cout << "析构" << endl;
    }
    void print(){
        cout << "(" << _x << "," << _y << ")" << endl;
    }
private:
    int _x;
    int _y;
};

void test(){
    Point &p1 = Point::getInstance();//这里加上引用是为了避免拷贝构造,解决办法是将拷贝构造放入私有强迫函数中使用引用
    Point &p2 = Point::getInstance();//若这里使用拷贝构造的话将创建出多个对象
    cout << &p1 << endl;
    cout << &p2 << endl;
}

int main(){
    test();
    return 0;
}

