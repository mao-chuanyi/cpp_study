#include "QueryResult.h"

ostream &print(ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << make_plural(qr.lines->size(), " time", "s") << endl; // qr.lines->size()是set<line_no>中的元素个数
    for (auto num : *qr.lines)                                                                                // 遍历set<line_no>中的元素
    {
        os << "\t(line " << num + 1 << ") " << *(qr.file->begin() + num) << endl; // 行号是从0开始记录的所以要+1
    }
    return os;
}