#include "TextQuery.h"

TextQuery::TextQuery(ifstream &ifs)
    : file(new vector<string>) // 调用shared_ptr的构造函数创建一个shared_ptr<vector<string>>对象
{
    string text;               // 存储一行的文本内容
    while (getline(ifs, text)) // 不断的读入每一行
    {
        file->push_back(text);    // 将读入的一行存入file
        int n = file->size() - 1; // 当前行号(从0开始存)
        istringstream line(text); // 字符串输入流
        string word;              // 存该行的逐个单词
        while (line >> word)      // 逐个处理该行的单词
        {
            auto &lines = wm[word];
            if (!lines) // 第一次遇到这个单词时lines为空
            {
                lines.reset(new set<line_no>); // shared_ptr重新管理一个新的对象
            }
            lines->insert(n); // 若单词已存在map中，则插入该单词所在的行号
        }
    }
}

QueryResult TextQuery::query(const string &sought) const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>); // 使用静态局部变量是因为不用每次没有找到的时候都去new一次
    auto loc = wm.find(sought);                               // 使用find()在map中查找，以免将不存在的单词插入map，返回值为map的迭代器
    if (loc == wm.end())
    {
        return QueryResult(sought, nodata, file); // 未找到
    }
    else
    {
        return QueryResult(sought, loc->second, file);
    }
}