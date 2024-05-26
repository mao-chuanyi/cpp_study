#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Computer{
public:
    Computer(const char * name, int id): 
        _name(new char[strlen(name)+1]()),
        _id(id)
    {
        strcpy(_name, name);
    }
    void print(){
        cout << "name = " << _name << endl;
        cout << "id = " << _id << endl;
    }
    ~Computer()
    {
        if(_name){//判断也是必要的，避免进行空指针的释放
            delete [] _name;
            _name = nullptr; //安全回收（必须）
        }
        cout << "~Computer()" << endl;
    }
private:
    char * _name;
    int _id;
};

void test(){
    Computer pc("apple", 20000);
    pc.~Computer();//不建议手动调用析构函数
    pc.print();
}

int main(){
    test();
    return 0;
}

