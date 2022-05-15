#ifndef __PAGEPREPROCESSOR_H__
#define __PAGEPREPROCESSOR_H__

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

using std::string;
using std::set;
using std::pair;
using std::vector;
using std::map;
using std::unordered_map;


class SplitTool;

class PagePreProcessor
{
public:
    PagePreProcessor();
    void invertProcess();
    void store();
    void dump();

private:
    unordered_map<string, set<pair<int, pair<double, int>>>> _invertIndexTable;//<关键字, <文档id, <w, 关键字在该文档出现的位置>>>
    map<string, int> _wordsFreq;//<关键字，包含此关键字的文档的数量>
    map<int, set<string>> _docWords;//<文档id, 该文档所拥有的关键字>
    unordered_map<string, unordered_map<int, pair<int, int>>> _docFreq;//<关键字， <包含此关键字的文档id，<在该文档出现的数量，在该文档出现的位置>>>
    SplitTool *_cuttor;
};

#endif