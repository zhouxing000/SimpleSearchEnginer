#ifndef __PAGEPRODUCER_H__
#define __PAGEPRODUCER_H__

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

struct RssItem
{
    string title;
    string link;
    string description;
};

struct Offset
{
    size_t idx;
    size_t startPos;
    size_t endPos;
};

class PageProducer
{
public:
    PageProducer(string);
    void buildripepage();
    void store();

    void initFileName(string);
    vector<string> getfiles(){
        return _files;
    }
private:
    vector<string> _files;
    vector<RssItem> _rss;
    vector<Offset> _offset;
};

#endif