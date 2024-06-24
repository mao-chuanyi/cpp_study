#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class TextQuery
{
public:
    TextQuery()
    {
        _lines.reserve(120);
    }
    void readFile(const string &filename)
    {
        ifstream ifs(filename);
        if(!ifs)
        {
            cerr << "file open if fail!" << endl;
            return;
        }
        string line;
        size_t lineNum = 0;//行号（从1开始）
        while(getline(ifs, line))
        {
            ++lineNum;
            _lines.push_back(line);//将该行存入vector
            istringstream iss(line);//字符串输入流(将字符串的内容传递给字符串输入流对象)
            string word;
            while (iss >> word)
            {
                //处理单词
                string newWord = dealWord(word);
                //插入字典
                ++_dict[newWord];
                //记录行号
                _wordNumbers[newWord].insert(lineNum);
                
            }
        }
        ifs.close();
    }

    string dealWord(string & word)
    {
        for(size_t i = 0; i != word.size(); ++i)
        {
            if(!isalpha(word[i]))
            {
                word[i] = ' ';
            }
            else if(isupper(word[i]))
            {
               word[i] = tolower(word[i]);
            }
        }
        return word;
    }

    void query(const string &word)
    {
        if(_dict[word] != 0)
        {
            cout << word << " occurs " << _dict[word] << " times." << endl;
            for(auto it = _wordNumbers[word].begin(); it != _wordNumbers[word].end(); ++it)
            {
                cout << "(line " << *it << ") " << _lines[(*it)-1] << endl;

            }
        }
    }


private:
    vector<string> _lines;//存放每一行（下标从0开始）
    map<string, set<int>> _wordNumbers;//存放每个单词出现在不同行的行号
    map<string, int> _dict;//记录每个单词出现的次数
};

void test(){
    string queryWord("chinese");

    TextQuery tq;
    tq.readFile("./china_daily.txt");
    tq.query(queryWord);
}

int main(){
    test();
    return 0;
}

