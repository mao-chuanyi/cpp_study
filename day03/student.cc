#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Students{
public:
    //赋值
    Students(int id, const char * name, int age):
        _id(id),
        _name(new char[strlen(name) + 1]),
        _age(age)
    {
        strcpy(_name, name);
    }

    //输出
    void print(){
        cout << "id : " << _id << endl;
        cout << "name : " << _name <<endl;
        cout << "age : " << _age << endl;
    }
    //析构函数
    ~Students(){
        if(_name){
            delete [] _name;
            _name = nullptr;
        }
        cout << "析构" << endl;
    }
private:
    int _id;
    char * _name;
    int _age;
};

void test(){
    Students st1(1, "maochuanyi", 25);
    st1.print();
}

int main(){
    test();
    return 0;
}

