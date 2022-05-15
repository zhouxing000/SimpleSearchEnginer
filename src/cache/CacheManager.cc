#include "/home/mikezhou/codes/SearchEnginerend/include/cache/CacheManager.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/cache/LRUCache.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/Configuration.h"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

using std::cout;
using std::endl;

CacheManager::CacheManager():_timer(3, 10, std::bind(&CacheManager::updateCache, this)) {
    init();
    _timer.start();
}

void CacheManager::init() {
    Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["cachelib"];

    for (int i = 0; i < 8; ++i) {
        _caches.push_back(LRUCache(10000));
    }
    _caches[0].readFromFile(str);
}

LRUCache& CacheManager::getCache(int idx) {
    cout << idx << " cache server" << endl;
    return _caches[idx];
}

void CacheManager::updateCache() {
    cout << "updating cache data" << endl;
    // for (int i = 1; i < _caches.size(); ++i) {
    //     string key;
    //     vector<string> value;
    //     for (auto it1 = _caches[i]._pendingUpdateList.begin();
    //          it1 != _caches[i]._pendingUpdateList.end(); ++it1) {
    //         key = (*it1)->key;
    //         for (auto it2 = (*it1)->value.begin(); it2 != (*it1)->value.end();
    //              ++it2) {
    //             value.push_back(*it2);
    //         }
    //         _caches[0]._lru.push_front(
    //             make_shared<LRUCache::CacheNode>(key, value));
    //         key.clear();
    //         value.clear();
    //     }
    // }

    // _caches[0].writeToFile("../data/cache.dat");

    // string key;
    // vector<string> value;
    // for (auto it1 = _caches[0]._lru.begin(); it1 != _caches[0]._lru.end();
    //      ++it1) {
    //     key = (*it1)->key;
    //     for (auto it2 = (*it1)->value.begin(); it2 != (*it1)->value.end();
    //          ++it2) {
    //         value.push_back(*it2);
    //     }
    //     for(int i = 1; i < _caches.size(); ++i){
    //         _caches[i]._lru.push_front(make_shared<LRUCache::CacheNode>(key, value));
    //     }
    //     key.clear();
    //     value.clear();
    // }

    for(int i = 1; i < _caches.size(); ++i){
        _caches[0]._isUpdating = true;
        _caches[i]._isUpdating = true;
        _caches[0].updateP2M(_caches[i]);
        _caches[0]._isUpdating = false;
        _caches[i]._isUpdating = false;
    }
    Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["cachelib"];
    _caches[0].writeToFile(str);

    for(int i = 1; i < _caches.size(); ++i){
        _caches[0]._isUpdating = true;
        _caches[i]._isUpdating = true;
        _caches[i].updateM2M(_caches[0]);
        _caches[0]._isUpdating = false;
        _caches[i]._isUpdating = false;
    }
}