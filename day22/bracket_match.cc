#include <iostream>
#include <string>
#include <stack>

using namespace std;

class Solution
{
public:
    bool isValid(string s)
    {
        stack<char> sta;
        for (auto &c : s)
        {
            if (c == '(' || c == '{' || c == '[') // 左括号
            {
                sta.push(c);
            }
            else // 右括号
            {
                if(sta.empty())
                {
                    return false;
                }
                if (sta.top() == '(' && c != ')')
                {
                    return false;
                }
                else if(sta.top() == '{' && c != '}')
                {
                    return false;
                }
                else if(sta.top() == '[' && c != ']')
                {
                    return false;
                }
                else
                {
                    sta.pop();
                }
            }
        }
        if(!sta.empty())
        {
            return false;
        }
        return true;
    }
};

void test()
{
    Solution solu;
    cout << solu.isValid("(") << endl;
}

int main()
{
    test();
    return 0;
}