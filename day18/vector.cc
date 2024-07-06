#include <iostream>
#include <vector>

using namespace std;

void test()
{
    //无参构造
    vector<int> v1;
    //count
    vector<int> v2(10);
    //count and value
    vector<int> v3(10, 1);
    //迭代器
    vector<int> v4(v3.begin(), v3.end());
    //大括号
    vector<int> v5{1,2,3,4,5,6};
    //拷贝构造
    vector<int> v6(v5);
    
    //遍历
    for(auto &v : v2)
    {
        cout << v << " ";
    }
    cout << endl;
    for(auto &v : v5)
    {
        cout << v << " ";
    }
    cout << endl;
}

int main()
{
    test();
    return 0;
}