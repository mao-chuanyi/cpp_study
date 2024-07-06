#include <iostream>
#include <vector>
#include <functional>

using namespace std;

template<typename T,typename Compare = less<T>> 
class MyQsort 
{ 
public:     
    MyQsort(T *arr, size_t size, Compare com = Compare())
    {
        cout << "test1" << endl;
       _vec.assign(arr, arr+size);
       quick(0, size-1, com);
    } 
    void quick(int left, int right, Compare &com)
    {
        cout << "test2" << endl;
        if(left >= right) 
        {
            return;
        }
        T mid = partition(left, right, com);
        quick(left, mid-1, com);
        quick(mid+1, right, com);
    }
    int partition(int left, int right, Compare &com)
    {
        cout << "test3" << endl;
        T tep = _vec[left];
        while(left < right)
        {
        cout << "test3.2" << endl;
            while(left < right && com(_vec[right], tep))
            {
                --right;
            }
            _vec[left] = _vec[right];
            while(left < right && !com( _vec[left], tep))
            {
                ++left;
            }
            _vec[right] = _vec[left];
        }
        _vec[left] = tep;
        return left;
    }
    void print()
    {
        cout << "test4" << endl;
        for(auto &v : _vec)
        {
            cout << v << " ";
        }
        cout << endl;
    }
private:    
    vector<T> _vec; 
};

void test1()
{
    int arr[] = {1,2,34,2,6,8,5,3,2,5, 1};
    MyQsort<int, greater<int>> sort(arr, sizeof(arr) / sizeof(arr[0]));
    sort.print();

}

int main()
{
    test1();
    return 0;
}
