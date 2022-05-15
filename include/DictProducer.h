#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>

using std::vector;
using std::pair;
using std::map;
using std::set;
using std::string;
using std::unique_ptr;

class SplitTool;

class DictProducer
{
public:
    DictProducer(string, SplitTool *);
    void buildEnDict();
    void buildCnDict();
    void createIndex();
    void store();

    void initFileName(string);
    vector<string> getfiles(){
        return _files;
    }
private:
    vector<string> _files;
    unique_ptr<vector<pair<string, int>>> _dict;
    unique_ptr<map<string, set<int>>> _index;
    SplitTool *_cuttor;
};

#endif