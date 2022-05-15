#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGET_H__

#include "LRUCache.h"
#include "Timerfd.h"
#include "TimerThread.h"

#include <vector>

using std::vector;

class LRUCache;

class CacheManager
{
public:
    friend class KeyRecommander;
    CacheManager();
    void init();
    LRUCache &getCache(int idx);
    void updateCache();

    void showCache(int idx);

private:
    TimerThread _timer;
    vector<LRUCache> _caches;
};

#endif