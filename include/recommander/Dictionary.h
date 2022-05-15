#ifndef __DICTIONARY_HPP__
#define __DICTIONARY_HPP__

#include "CandidateResult.h"



#include <vector>
#include <string>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

class Dictionary
{
public:
    Dictionary();

    vector<string> doQuery(const string &);
    void queryIndex();
    int minDistance(string);
    
    void initDict();
    void initIndex();
    void show_dict();
    void show_index();

    void getRandKeys(vector<string> &keys);
    void getSolitaireKeys(string &key, vector<string> &keys);
    size_t nBytesCode(const char ch);
    size_t length(const std::string &str);

    vector<string> splitSought(string);

private:
    vector<pair<string, int>> _dict;
    map<string, set<int>> _index;

};

#endif