#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include <string>
#include <vector>
#include <queue>

using std::string;
using std::vector;
using std::priority_queue;

class CandidataResult;
// class TcpConnectionPtr;
class Dictionary;
class CacheManager;

class KeyRecommander
{
public:
    KeyRecommander(Dictionary &dic, CacheManager &cachemanager);
    void doQuery(string sought, int idx, vector<string> &res);
    void getRandDict(vector<string> &);
    void getSolitaireKeys(string &key, vector<string> &keys);
private:
    Dictionary &_dic;
    CacheManager &_cachemanager;
    string _sought;
    // priority_queue<CandidataResult> _prique;
    // TcpConnectionPtr _conn;
};

#endif
