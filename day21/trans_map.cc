#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::string;

class Trans
{
public:
    void read_transform(const string &filename) // 将转换规律存入map
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cerr << "file open erro" << endl;
        }
        string key;
        string value;
        while (ifs >> key && getline(ifs, value))
        {
            value = value.substr(1); // 去掉前面的空格
            _tansform[key] = value;
        }
    }
    void transform(const string &filename)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cerr << "file open erro" << endl;
        }
        string line;
        while (getline(ifs, line))
        {
            istringstream iss(line);
            string word;
            while (iss >> word)
            {
                if (_tansform.count(word) != 0)
                {
                    cout << _tansform[word] << " ";
                }
                else
                {
                    cout << word << " ";
                }
            }
            cout << endl;
        }
    }
    void printmap()
    {
        for (auto &m : _tansform)
        {
            cout << m.first << " " << m.second << endl;
        }
    }

private:
    map<string, string> _tansform;
};

void test()
{
    Trans ts;
    ts.read_transform("./map.txt");
    ts.transform("./file.txt");
}
int main()
{
    test();
    return 0;
}