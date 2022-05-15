#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <unordered_map>

using std::string;
using std::vector;
using std::list;
using std::unordered_map;
using std::unique_ptr;
using std::make_unique;

class LRUCache
{
public:
    friend class CacheManager;
    friend class KeyRecommander;
    LRUCache(int capacity):_capacity(capacity),_isUpdating(false) {}
    
    bool get(string key, vector<string> &val);
    
    void put(string key, vector<string> value);

    void writeToFile(string filename);
    void readFromFile(string filename);

    void updateP2M(LRUCache &);
    void updateM2M(LRUCache &);

    void show();
private:
    struct CacheNode {
        string key;
        vector<string> value;
        CacheNode(string k, vector<string> v)
        : key(k), value(v)
        {}
    };
    unordered_map<string, list<unique_ptr<CacheNode>>::iterator> _umap;
    list<unique_ptr<CacheNode>> _lru;
    list<unique_ptr<CacheNode>> _pendingUpdateList;
    int _capacity;
    bool _isUpdating;
};

#endif
