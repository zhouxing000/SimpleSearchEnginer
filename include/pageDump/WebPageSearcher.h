#ifndef __WEBPAGESEARCHER_H__
#define __WEBPAGESEARCHER_H__

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using std::string;
using std::vector;
using std::set;
using std::unordered_map;
using std::pair;

struct WebPage{
    int id;
    string title;
    string link;
    string description;
    double cos = 0.0;

    bool operator<(const WebPage &rhs) const{
        return cos > rhs.cos;
    }
};

class WebPageSearcher
{
public:
    WebPageSearcher();
    void initInvertIndex();
    void initOffset();
    void doQuery(string, string &res);
    void singleQuery(string, vector<int> &);
    void multiQuery(const vector<string> &, vector<int> &);
    void simCalc(vector<string> &, vector<int> &, set<WebPage> &);
    void getWebPageSet(int id, double cos, set<WebPage>& webPageSet);
    void showRes(const vector<string> &keys, const set<WebPage> &webPageSet);
    void outRes(const vector<string> &keys, const set<WebPage> &webPageSet, string &res);

private:
    size_t nBytesCode(const char ch);

private:
    unordered_map<int, pair<int, int>> _offset;//<文档id, <文档开始位置, 文档结束位置>>
    unordered_map<string, set<pair<int, pair<double, int>>>> _invertIndexTable;//<关键字, <文档id, <w, 关键字在该文档出现的位置>>>
};

#endif