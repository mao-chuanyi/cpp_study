#ifndef QUERY_H
#define QUERY_H
#include "QueryResult.h"
#include "TextQuery.h"
#include <iostream>
#include <memory>
#include <algorithm>
using namespace std;

class Query_base
{
    friend class Query; // 因为Query需要调用Query_base的虚函数，所以设置为友元
protected:
    using line_no = TextQuery::line_no; // 定义一个变量类型的别名
    virtual ~Query_base() = default;    // 默认虚析构函数，确保派生类的析构能够成功调用

private:
    virtual QueryResult eval(const TextQuery &) const = 0; // 获得查询结果
    virtual string rep() const = 0;                        // 生成string的查询版本？
};

// 接口类，负责隐藏整个继承体系
class Query
{
    // 因为这些运算符需要访问私有的构造函数，所以设置为友元
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);

public:
    Query(const string &); // 构建一个新的WordQuery对象

    // 接口函数， 调用对应的Query_base操作
    QueryResult eval(const TextQuery &t) const
    {
        return q->eval(t);
    }
    string rep() const
    {
        cout << "Query::rep()" << endl;
        return q->rep();
    }

private:
    Query(shared_ptr<Query_base> query) // 私有的构造函数，用于绑定派生类的对象，用于支持不同运算符去创建对象
        : q(query)
    {
        cout << "Query(shared_ptr<Query_base>)" << endl;
    }
    shared_ptr<Query_base> q;
};

class WordQuery
    : public Query_base
{
    friend class Query; // Query需要访问WordQuery的构造函数
    WordQuery(const string &s)
        : query_word(s)
    {
        cout << "WordQuery(const string &)" << endl;
    }
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);
    }

    string rep() const // 返回要查询的单词
    {
        cout << "WordQuery::rep()" << endl;
        return query_word;
    }

    string query_word; // 要查询的单词
};

class NotQuery
    : public Query_base
{
    friend Query operator~(const Query &);
    NotQuery(const Query &q)
        : query(q)
    {
        cout << "NotQuery(const Query &)" << endl;
    }
    QueryResult eval(const TextQuery &) const;
    string rep() const
    {
        cout << "NotQuery::rep()" << endl;
        return "~(" + query.rep() + ")";
    }
    Query query;
};

class BinaryQuery
    : public Query_base
{
protected:
    BinaryQuery(const Query &l, const Query &r, const string &op)
        : _lhs(l), _rhs(r), _opSam(op)
    {
        cout << "BinaryQuery(const Query&, const Query&, const string)" << endl;
    }
    // 这里不定义eval函数,因为BiaryQuery还是一个抽象函数，但是继承了基类的接口约束

    string rep() const
    {
        cout << "BinaryQuery::rep()" << endl;
        return "(" + _lhs.rep() + _opSam + _rhs.rep() + ")";
    }
    Query _lhs, _rhs; // 左侧和右侧运算对象
    string _opSam;    // 运算符
};

class OrQuery
    : public BinaryQuery
{
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &left, const Query &right)
        : BinaryQuery(left, right, "|")
    {
        cout << "OrQuery(const Query&, const Query&)" << endl;
    }
    QueryResult eval(const TextQuery &) const;
    // rep已经继承了BinaryQuery中定义的rep
};
class AndQuery
    : public BinaryQuery
{
    friend Query operator&(const Query &, const Query &);
    AndQuery(const Query &left, const Query &right)
        : BinaryQuery(left, right, "&")
    {
        cout << "AndQuery(const Query&, const Query&)" << endl;
    }
    QueryResult eval(const TextQuery &) const;
    // rep已经继承了BinaryQuery中定义的rep
};

inline Query::Query(const string &s)
    : q(new WordQuery(s))
{
    cout << "Query(const sting&)" << endl;
}
inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand)); // 这里隐式的使用了Query(shared_ptr<Query_base>)构造函数
}
inline Query operator|(const Query &lq, const Query &rq)
{
    return shared_ptr<Query_base>(new OrQuery(lq, rq)); // 这里也隐式调用了Query(shared_ptr<Query_base>)构造函数
}
inline Query operator&(const Query &lq, const Query &rq)
{
    return shared_ptr<Query_base>(new AndQuery(lq, rq)); // 这里也隐式调用了Query(shared_ptr<Query_base>)构造函数
}
#endif