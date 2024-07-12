#include <iostream>
#include <memory>
using namespace std;

template <typename T>
class Vector
{ 
public:     
    Vector();     
    ~Vector();          
    void push_back(const T &);      
    void pop_back();         
    int size();     
    int capacity(); 
private:     
    void reallocate();//重新分配内存,动态扩容要用的 
private:         
    static std::allocator<T> _alloc;          
    T *_start;                 //指向数组中的第一个元素     
    T *_finish;                //指向最后一个实际元素之后的那个元素     
    T *_end_of_storage;        //指向数组本身之后的位置 
};

template <typename T>
Vector<T>::Vector()
:_start(nullptr), _finish(nullptr), _end_of_storage(nullptr)
{
    cout << "Vector()" << endl;
}

template <typename T>
void Vector<T>::push_back(const T &value)
{
    if(size() == capacity())
    {
        reallocate();
    }
    _alloc.construct(_finish++, value);
}

template <typename T>
void Vector<T>::pop_back()
{
    if(size() != 0)
    {
        _alloc.destor(--_finish);
        _alloc.deallocate(_finish, 1);
    }
    else
    {
        cout << "empty!" << endl;
    }
}

template <typename T>
void Vector<T>::reallocate()
{
    int oldcapacity = capacity();
    int newcapacity = oldcapacity == 0 ? 1 : 2 * oldcapacity;
    T* nptr = _alloc.allocate(newcapacity);
    if(_start)//已经有数据的情况下
    {
        //将老空间的内容拷贝过来
        uninitialized_copy(_start, _finish, nptr);
        //将旧空间回收释放
        T* tep_start = _start;
        while(_start != _finish)
        {
            _alloc.destroy(_start++);//销毁对象
        }
        _alloc.deallocate(tep_start, capacity());//回收空间
    }
    //重置指针
    _start = nptr;
    _finish = _start + oldcapacity;
    _end_of_storage = _start + newcapacity;
}

template <typename T>
Vector<T>::~Vector()
{
    if(_start)
    {
        T* tep_start = _start;
        while(_start != _finish)
        {
            _alloc.destory(_start++);
        }
        _alloc.deallocate(tep_start, capacity());
    }
}
template <typename T>
int Vector<T>::size()
{
    return _finish - _start;
}

template <typename T>
int Vector<T>::capacity()
{
    return _end_of_storage - _start;
}
