#include <string.h>
#include <iostream>
using namespace std;

class String
{
public:
	String();
	String(const char *pstr);
	String(const String &rhs);
    String &operator=(const String &rhs);
    ~String();
    void print();
   // size_t length() const;
   // const char * c_str() const;

private:
	char * _pstr;
};
//无参构造
String::String():
    _pstr(nullptr)
{
    cout << "无参构造" << endl;
}

//有参构造（传入一个字符串）
String::String(const char *pstr):
    _pstr(new char[strlen(pstr)+1])
{
    strcpy(_pstr, pstr);
    cout << "有参构造" << endl;
}

//拷贝构造（传入一个String对象）
String::String(const String &rhs):
    _pstr(new char[strlen(rhs._pstr) + 1])
{
    strcpy(_pstr, rhs._pstr);
    cout << "拷贝构造" << endl;
}
//赋值运算符函数
String &String::operator=(const String &rhs){
    delete [] _pstr;
    _pstr = new char[strlen(rhs._pstr) + 1];
    strcpy(_pstr, rhs._pstr);
    return *this;
}

//打印
void String::print()
{
    if(_pstr){
        cout << _pstr << endl;
    }else{
        cout << "nullptr" << endl;
    }
}

//析构
String::~String()
{
    if(_pstr)
    {
        delete [] _pstr;
        _pstr = nullptr;
    }
    cout << "析构" << endl;
}
int main()
{
	String str1;
	str1.print();
	

	String str2 = "Hello,world";
	String str3("wangdao");
	
    str2.print();		
	str3.print();	
	
	String str4 = str3;
	str4.print();
	
	str4 = str2;
	str4.print();
	
	return 0;
}
