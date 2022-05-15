#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/PagePreProcessor.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/PageProducer.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/SplitTool.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.hpp"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
    // PageProducer pp("../m");
    // pp.buildripepage();
    // pp.store();
    // SplitTool *psp = new SplitTool();
    PagePreProcessor *pre = new PagePreProcessor();
    pre->invertProcess();
    // pre->dump();
    pre->store();

    // string str;
    // string res;
    // getline(cin, str);
    // WebPageSearcher searcher;
    // searcher.doQuery(str, res);
    // // searcher.dump();
    // cout << res << endl;
    return 0;
}

