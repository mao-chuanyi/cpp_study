#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;
using std::unordered_map;

class Dictionary
{
public:
    // 读入文件
    void read(const string &filename)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cerr << "ifs open file fail" << endl;
            return;
        }
        string line;
        while (getline(ifs, line))
        {
            istringstream iss(line);
            string word;
            while (iss >> word)
            {
                string newword = dealword(word);
                insertword(newword);
            }
        }
    }
    string dealword(string &word)
    {
        for (size_t i = 0; i < word.size(); ++i)
        {
            if (!isalpha(word[i]))
            {                    // 判断当前字符是否为字母
                return string(); // 不是合法单词就返回空字符串
            }
            else if (isupper(word[i]))
            {
                word[i] = tolower(word[i]);
            }
        }
        return word;
    }
    void insertword(const string &word)
    {
        if (word != string())
        {
            ++_dict[word];
        }
        else
        {
            return;
        }
    }
    void display()
    {
        for (auto &dict : _dict)
        {
            cout << dict.first << " " << dict.second << endl;
        }
    }

private:
    unordered_map<string, int> _dict;
};

void test()
{
    Dictionary dict;
    dict.read("./china_daily.txt");
    dict.display();
}

int main()
{
    test();
    return 0;
}
