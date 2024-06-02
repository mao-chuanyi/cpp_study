#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

void test(){
    vector<vector<int>> v;
    v.push_back({1,2,3,4,5});
    v.push_back({2,3,4,5,6});
    v.push_back({3,4,5,6,7});

    //输出
//    for (vector<vector<int>>::iterator it = v.begin(); it != v.end(); ++it)
//    {
//        for(vector<int>::iterator vit = (*it).begin(); vit != (*it).end(); ++vit)
//        {
//            cout << (*vit) <<" ";
//        }
//        cout << endl;
//    }
    //c++11引入的范围循环
    for (const auto &row : v)
    {
        for (const auto &elem : row)
        {
            cout << elem << " ";
        }
        cout << endl;
    }
}

int main(){
    test();
    return 0;
}

