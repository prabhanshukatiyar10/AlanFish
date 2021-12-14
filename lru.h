#include "defs.h"
class LRU
{
    list< pair<U64,int> > item_list;
    unordered_map<U64, decltype(item_list.begin()) > item_map;
    size_t cache_size;

    void clean();
    public:
        LRU();
        LRU(int size);
        void put(const U64 &key, const int &val);
        bool exist(const U64 &key);
        int get(const U64 &key);
};