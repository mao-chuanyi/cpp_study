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

    ~Point(){//访问权限和构造函数保持一致甚至更严
        cout << "析构" << endl;
    }


public:
    static Point * getInstance(){
        if(_p == nullptr){//若没有这个判断那么每调用一次都会new一个新的对象出来
            _p =new Point(0,0); 
        }
        return _p;
    }

    void init(int x, int y){
        if(_p){
            _x = x;
            _y = y;
        }
    }
    static void delInstance(){//要和析构函数区分开，析构函数是用于回收对象的数据成员申请的堆空间
                              //写成静态成员函数的原因是为了让形式好看易理解
                              //如果是普通成员函数的话就要写成：Point::getInstance(1,2)->delInstance();
        if(_p){
            delete _p;
            _p = nullptr;
        }
    }

    void setInstance(int x, int y){
        if(_p){
            _p->_x = x;
            _p->_y = y;
        }
    }

    
    void print(){
        cout << "(" << _p->_x << "," << _p->_y << ")" << endl;
    }

    Point(const Point & p) = delete;
    Point & operator=(const Point & p) = delete;

private:
    int _x;
    int _y;
    static Point *_p;//用来判断是否是否已经再对上创建对象（非空），又因为静态函数无法访问非静态数据成员，所以需要将其设置为静态数据成员
};

//静态变量内外初始化
Point * Point::_p = nullptr;  

void test(){
    Point::getInstance()->init(1,2);//这才是单例对象的规范而不是用一个指针来接收，避免创建出多个指针
    Point::getInstance()->print();
    Point::getInstance()->setInstance(3,5);
    Point::getInstance()->print();
    Point::delInstance();
}

int main(){
    test();
    return 0;
}

