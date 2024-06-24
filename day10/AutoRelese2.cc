#include <iostream>
using namespace std;

//单例类
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if(nullptr == _pInstance)
        {
            //使用构造函数
            _pInstance = new Singleton();
        }
        return _pInstance;
    }

private:
    class AutoRelese
    {
    public:
        AutoRelese()
        {
            cout << "AutoRelese()" << endl;
        }

        ~AutoRelese()
        {
            cout << "~AutoRelese()" << endl;
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }

    ~Singleton()
    {
        cout << " ~Singleton()" << endl;
    }

private:
    static Singleton *_pInstance;
    static AutoRelese _ar;//对象数据成员

};

Singleton *Singleton::_pInstance = nullptr;
Singleton::AutoRelese Singleton::_ar;//创建静态对象


int main(int argc, char * argv[]){
    Singleton *ps1 = Singleton::getInstance();
    return 0;
}
