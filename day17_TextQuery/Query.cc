#include "Query.h"

QueryResult NotQuery::eval(const TextQuery &t) const
{
    auto result = query.eval(t);
    auto ret_lines = make_shared<set<line_no>>();
    auto beg = result.begin();
    auto end = result.end();//此处的end是最后一个数的下一个，即end没有指向实质性的数
    size_t sz = result.get_file()->size();
    for (size_t i = 0; i != sz; ++i)
    {
        if (beg == end || *beg != i)
        {
            ret_lines->insert(i);
        }
        else if (beg != end)
        {
            ++beg;
        }
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &t) const
{
    auto left = _lhs.eval(t); // 获得左操作数的QueryResult
    auto right = _rhs.eval(t);
    auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end()); // 创建一个set<line_no>类型的shared_ptr来存储两个操作数获得的行号
    ret_lines->insert(right.begin(), right.end());                        // 将左右两个操作数的页号合并（取并集）
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery &t) const
{
    auto left = _lhs.eval(t);
    auto right = _rhs.eval(t);
    auto ret_lines = make_shared<set<line_no>>();
    set_intersection(left.begin(), left.end(), right.begin(), right.end(), inserter(*ret_lines, ret_lines->begin())); // 利用algorithm中的函数来对两个set合并（取交集）
    return QueryResult(rep(), ret_lines, left.get_file());
}