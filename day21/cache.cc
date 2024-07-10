#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class LRUCache
{
public:
    LRUCache(int capacity) : _cap(capacity) {}

    int get(int key)
    {
        if (_hashMap.count(key) == 0) // 未找到
        {
            return -1;
        }
        else // 找到，将其移动至表头
        {
            _cache.splice(_cache.begin(), _cache, _hashMap[key]);
            _hashMap[key] = _cache.begin();
            return _hashMap[key]->second;
        }
    }

    void put(int key, int value)
    {
        if (get(key) != -1)
        {
            _hashMap[key]->second = value;
        }
        else
        {
            if (_cache.size() == _cap) // 缓存满，删除链表尾（最近最少使用）
            {
                cout << "full size = " << _cache.size() << endl;
                _cache.push_front(make_pair(key, value)); // 插入表头
                _hashMap.insert({key, _cache.begin()});   // 插入哈希表
                for (auto it = _cache.begin(); it != _cache.end(); ++it)
                {
                    cout << "{" << it->first << "," << it->second << "}" << endl;
                }
                cout << (--(_cache.end()))->first << endl;
                _hashMap.erase((--_cache.end())->first); // 在哈希表中删除最后一个节点（最近最久未访问）
                _cache.pop_back();
            }
            else // 缓存没有满，插入链表头（最近访问）
            {
                cout << "not full size = " << _cache.size() << endl;
                _cache.push_front(make_pair(key, value)); // 插入表头
                _hashMap.insert({key, _cache.begin()});   // 插入哈希表
            }
        }
    }

private:
    int _cap;
    list<pair<int, int>> _cache;
    unordered_map<int, list<pair<int, int>>::iterator> _hashMap;
};

void test()
{
    LRUCache *obj = new LRUCache(2);
    obj->put(2, 1);
    obj->put(2, 2);
    cout << obj->get(2) << endl;
}

void test02()
{
    LRUCache *lRUCache = new LRUCache(2);
    lRUCache->put(2, 1); // 缓存是 {1=1}
    lRUCache->put(1, 1); // 缓存是 {1=1, 2=2}
    lRUCache->put(2, 3);
    lRUCache->put(4, 1);
    int i = lRUCache->get(1); // 返回 1
    cout << i << endl;
    i = lRUCache->get(2); // 返回 -1 (未找到)
    cout << i << endl;
}

int main()
{
    test02();
    return 0;
}