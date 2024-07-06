#include <iostream>
#include <set>

using namespace std;

void print(const set<int> &s)
{
    for (auto &s1 : s)
    {
        cout << s1 << " ";
    }
    cout << endl;
}
int main()
{
    set<int> s1{1, 3, 4, 4, 9, 2};
    print(s1);
    set<int> s2{4, 2, 5, 1, 5, 3, 6};
    print(s2);
    if (s1 < s2)
        cout << "<" << endl;
    else
        cout << ">" << endl;
}