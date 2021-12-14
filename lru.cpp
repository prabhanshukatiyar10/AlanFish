#include "lru.h"

LRU::LRU()
{

}

LRU::LRU(int sz)
{
    cache_size = sz;
}

void LRU::clean()
{
    while(item_map.size()>cache_size)
    {
        auto it = item_list.end();
        it--;
        item_map.erase(it->first);
        item_list.pop_back();
    }
}
void LRU::put(const U64 &key, const int &val)
{
    auto it = item_map.find(key);
    if(it!=item_map.end())
    {
        item_list.erase(it->second);
        item_map.erase(it);
    }
    item_list.push_front(make_pair(key,val));
    item_map.insert(make_pair(key, item_list.begin()));
    clean();
}
bool LRU::exist(const U64 &key)
{
    return (item_map.find(key)!=item_map.end());
}

int LRU::get(const U64 &key)
{
    auto it = item_map.find(key);
    if(it==item_map.end())
        return 0;
    item_list.splice(item_list.begin(), item_list, it->second);
    return it->second->second;
}