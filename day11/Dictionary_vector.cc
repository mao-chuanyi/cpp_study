#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

struct Record
{
    Record(const string &word, int frequency):
        _word(word),
        _frequency(frequency)
    {}
    string _word;
    int _frequency;
};

class Dictionary
{
public:
    Dictionary(int capacity){
        _dict.reserve(capacity);
    }
    void read(const string &filename)//从文件读
    {
        ifstream ifs(filename);
        if(!ifs){
            cerr << "ifs open file fail" << endl;
            return;
        }
        //不要一个单词一个单词的读入，因为会进行很多次慢速的磁盘IO（效率低）
        //按行读取
        string line;
        while(getline(ifs, line))
        {
            istringstream iss(line);
            string word;
            while (iss >> word){
                //处理单词
                string newWord = dealword(word);
                //将单词插入vector
                insertWord(newWord);
            }
        }
        ifs.close();
    }

    string dealword(string & word)
    {
        for(size_t i = 0; i != word.size(); ++i)
        {
            if(!isalpha(word[i])){//判断当前字符是否为字母
                return string();//不是合法单词就返回空字符串
            }else if(isupper(word[i])){
                word[i] = tolower(word[i]);
            }
        }
        return word;
    }

    void insertWord(const string & word)
    {
        if(word == string()){
            return;
        }
        size_t i = 0;
        for(; i != _dict.size(); ++i)
        {
            if(word == _dict[i]._word){
                ++_dict[i]._frequency;
                break;
            }
        }
        if(i == _dict.size())
        {
            _dict.push_back(Record(word,1));
        }
    }

    
    void print()
    {
        for(auto & dict : _dict)
        {
            cout << dict._word << " " << dict._frequency << endl;
        }
    }


    void store(const string &filename)//将vector的内容写入文件
    {
        ofstream ofs(filename);
        if(!ofs)
        {
            cerr << "ofs open file fail" << endl;
            return;
        }
        for(size_t i = 0; i != _dict.size(); ++i)
        {
            ofs << _dict[i]._word << " " << _dict[i]._frequency << endl;
        }
        ofs.close();
    }
private:
    vector<Record> _dict;
};

void test(){
    Dictionary dict(10000);
    dict.read("./The_Holy_Bible.txt");
    dict.store("dict.txt");
}

int main(){
    test();
    return 0;
}

