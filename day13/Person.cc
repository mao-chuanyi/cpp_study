#include <iostream>
#include <string.h>
using namespace std;

class Person
{
public:
    Person(const char * name, int age)
        :_name(new char[strlen(name)+1])
        ,_age(age)
    {
        strcpy(_name, name);
        cout << "Person(const char *, int)" << endl;
    }
    Person(const Person & rhs)
        :_name(new char[strlen(rhs._name)+1])
        ,_age(rhs._age)
    {
        strcpy(_name, rhs._name);
        cout << "Person(const Person &)" << endl;
    }
    Person & operator=(const Person & rhs)
    {
        if(this != &rhs)
        {
            delete [] _name;
            _name = nullptr;
            _name = new char[strlen(rhs._name) + 1];
            strcpy(_name, rhs._name);
            _age = rhs._age;
        }
        cout << "Person::operator=" << endl;
        return *this;
    }

    virtual void display () const
    {
        cout << "name : " << _name << endl;
        cout << "age : " << _age << endl;
    }


    virtual ~Person()
    {
        if(_name != nullptr)
        {
            delete [] _name;
            _name = nullptr;
        }
        cout << "~Person()" << endl;
    }

private:
    char * _name;
    int _age;
};

class Employee
:public Person
{
public:
    Employee(const char * name, int age, const char * department, double salary)
        :Person(name, age)
        ,_department(new char[strlen(department)+1])
        ,_salary(salary)
    {
        strcpy(_department, department);
        cout << "Employee(const char *, int , const char *, double)" << endl;
    }
    Employee(const Employee & rhs)
        :Person(rhs)
        ,_department(new char[strlen(rhs._department) + 1])
        ,_salary(rhs._salary)
    {
        strcpy(_department, rhs._department);
        cout << "Employee(const Employee &)" << endl;
    }
    Employee & operator=(const Employee & rhs)
    {
        if(this != &rhs)
        {
            delete [] _department;
            _department = nullptr;
            Person::operator=(rhs);
            _department = new char[strlen(rhs._department) + 1]();
            strcpy(_department, rhs._department);
            _salary = rhs._salary;
        }
        cout << "Employee::operator=" << endl;
        return *this;
    }
    ~Employee()
    {
        if(_department != nullptr)
        {
            delete [] _department;
            _department = nullptr;
        }
        cout << "~Employee()" << endl;
    }
    void display () const override 
    {
        Person::display();
        cout << "department : " << _department << endl;
        cout << "salary : " << _salary << endl;
    }
private:
    char * _department;
    double _salary;
};


void test(){
    Employee e1("mao", 24, "HR", 9000);
    Employee e2(e1);
    Employee e3("chroye", 25, "IT", 10000);
    cout << "e1 : " << endl;
    e1.display();
    cout << "e2 : " << endl;
    e2.display();
    cout << "e3 : " << endl;
    e3.display();
    e1 = e3;
    cout << "e1 : " << endl;
    e1.display();
    
}

int main(){
    test();
    return 0;
}

