#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include "QueryResult.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
using namespace std;

class QueryResult; // 前置声明

class TextQuery
{
public:
    using line_no = vector<string>::size_type; // 起别名方便见名知意,v.size()的返回值类型就是vector<string>::size_type
    TextQuery(ifstream &);                     // 构造函数,主要工作是将文件的内容读入并存储起来
    QueryResult query(const string &) const;   // 负责查询的成员函数

private:
    shared_ptr<vector<string>> file;          // file是一个指向vector<string>类型的智能指针，用于存储每一行内容
    map<string, shared_ptr<set<line_no>>> wm; // 字母作为键，指向set<line_no>类型的智能指针作为值，用于存储单词以及所在行号
};
#endif