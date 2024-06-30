#include "Query.h"

void runQueries(ifstream &ifs)
{
    TextQuery tq(ifs); // 传入文件输入流创建一个文件查询对象
    while (true)
    {
        cout << "enter two word to look for, or \"q q\" to quit: ";
        string s1;
        string s2;
        if (!(cin >> s1 >> s2) || s1 == "q")
        {
            break;
        }
        Query andq = Query(s1) | Query(s2);//andq中的shared_ptr<Query_base>类型的数据成员指向AndQuery对象
        print(cout, andq.eval(tq)) << endl; // 文件查询对象调用查询函数，并打印结果
    }
}

int main()
{
    ifstream ifs("./china_daily.txt"); // 创建一个文件输入流
    if (!ifs)                          // 判断是否正确打开文件
    {
        cerr << "file open error!" << endl;
        return 0;
    }
    runQueries(ifs); // 开始运行查询任务
    return 0;
}