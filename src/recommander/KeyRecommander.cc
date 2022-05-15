#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/KeyRecommander.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/Dictionary.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/cache/CacheManager.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/cache/LRUCache.h"

#include <iostream>

using std::cout;
using std::endl;

KeyRecommander::KeyRecommander(Dictionary &dic,CacheManager &cachemanager)
:_dic(dic)
,_cachemanager(cachemanager)
{}

void KeyRecommander::doQuery(string sought, int idx, vector<string> &res){
    _sought = sought;
    cout << "_sought:" << _sought << endl;
    bool flag = _cachemanager._caches[idx]._isUpdating;
    LRUCache &lru = _cachemanager.getCache(idx + (flag ? 4 : 0)); 
    lru.show();
    if(lru.get(_sought, res)){
        cout << "find data from cache" << endl;
        return;
    }
    cout << "find not data from cache" << endl;
    res = _dic.doQuery(_sought);
    lru.put(_sought, res);
    // cout << _sought << endl;
}

void KeyRecommander::getRandDict(vector<string> &keys){
    _dic.getRandKeys(keys);
}

void KeyRecommander::getSolitaireKeys(string &key, vector<string> &keys){
    _dic.getSolitaireKeys(key, keys);
}