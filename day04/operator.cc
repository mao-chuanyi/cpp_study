#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

class Computer{
public:
    //构造函数
    Computer(const char * name, int id): 
        _name(new char[strlen(name)+1]()),
        _id(id)
    {
        strcpy(_name, name);
        cout << "构造函数" << endl;
    }


    void print(){
        cout << "name = " << _name << endl;
        cout << "id = " << _id << endl;
    }

//    //拷贝构造函数(浅拷贝)
//    Computer(const Computer & pc):
//        _name(pc._name),//这里相当于_name = pc._name，两个指针指向同一片地址空间
//        _id(pc._id)
//    {
//        cout << "Computer(const Computer&pc)" << endl;
//    }

    //深拷贝
    Computer( Computer & pc):
        _name(new char[strlen(pc._name)+1]),
        _id(pc._id)
    {
        strcpy(_name, pc._name);
        cout << "深拷贝" << endl;
    }


    //赋值运算符函数(深拷贝)
    Computer & operator=(const Computer & pc)
    {
        if(this != &pc)//避免自己给自己赋值
        {
            delete [] _name;//先释放要被覆盖的对象在堆上申请的空间
            _name = new char[strlen(pc._name) + 1]();
            strcpy(_name,pc._name);
            _id = pc._id;
        }
        cout << "赋值运算符函数" << endl;
        return *this;
    }

    //析构函数
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
    Computer pc2("xiaomi", 6000);
    pc2 = pc;
    pc2.print();
    Computer pc3((Computer("Huawei", 8000)));
    pc3.print();
}

int main(){
    test();
    return 0;
}

