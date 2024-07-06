#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename Compare = std::less<T>>
class HeapSort
{
public:
    HeapSort(T *arr, size_t size)
    {
        _vec.reserve(size + 1);
        auto it = _vec.begin();
        _vec.insert(it, 1);
        _vec.insert(it + 1, arr, arr + size);
        sort();
    }

    void heapAdjust(size_t k, size_t len)//less<T>建立的是一个大根堆
    {
        _vec[0] = _vec[k]; // 防止被覆盖
        for (size_t i = 2 * k; i <= len; i *= 2)
        {
            if (i < len && _cmp(_vec[i], _vec[i + 1])) // i < i+1(less)(比较两个分支谁大)
            {
                ++i;
            }
            if (_cmp(_vec[0], _vec[i])) // 0 < i(less)(需要下坠)
            {
                _vec[k] = _vec[i];
                k = i;
            }
            else // 不用下坠
            {
                break;
            }
        }
        _vec[k] = _vec[0];
    }

    void sort()
    {
        //调整为一个堆
        size_t len = _vec.size() - 1;
        for (size_t i = len/2; i > 0; i--)
        {
            heapAdjust(i, len);
        }
        //排序
        for (size_t i = len; i > 1; --i)
        {
            swap(_vec[1], _vec[i]);//将第一个和最后一个交换
            heapAdjust(1, i-1);//下坠
        }
    }

    void print()
    {
        auto it = _vec.begin();
        ++it;
        for(; it != _vec.end(); ++it)
        {
            cout << *it << " ";
        }
        cout << endl;
    }

private:
    vector<T> _vec;
    Compare _cmp;
};

void test01()
{
    int arr[] = {3, 4, 6, 3, 67, 2, 8, 3, 0, 5, 4};
    HeapSort<int> hs(arr, sizeof(arr) / sizeof(arr[0]));
    hs.print();
}

int main()
{
    test01();
    return 0;
}