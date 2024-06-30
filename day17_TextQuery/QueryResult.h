#ifndef QUERYRESULT_H
#define QUERYRESULT_H
#include "make_plural.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <vector>
#include <string>
using namespace std;

class QueryResult
{
    friend ostream &print(ostream &, const QueryResult &); // 友元声明，用于访问私有成员

public:
    using line_no = vector<string>::size_type;                                      // 别名
    using line_it = set<line_no>::const_iterator;
    QueryResult(string s, shared_ptr<set<line_no>> p, shared_ptr<vector<string>> f) // 构造函数
        : sought(s), lines(p), file(f)
    {
    }
    line_it begin()
    {
        return lines->begin();
    }
    line_it end()
    {
        return lines->end();
    }
    shared_ptr<vector<string>> get_file()
    {
        return file;
    }

private:
    string sought;                   // 要查询的单词
    shared_ptr<set<line_no>> lines;  // 单词出现的行号
    shared_ptr<vector<string>> file; // 单词所在行
};

ostream &print(ostream &, const QueryResult &);
#endif