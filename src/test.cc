#include "/home/mikezhou/codes/SearchEnginerend/include/cache/CacheManager.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/Dictionary.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/KeyRecommander.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/cache/LRUCache.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/EchoServer.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/DataBag.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.hpp"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void showResult(vector<CandidateResult> cr) {
    for (int i = 0; i < cr.size(); ++i) {
        cout << cr[i]._word << ":" << endl
             << "dis:" << cr[i]._dis << endl
             << "fre:" << cr[i]._fre << endl;
    }
}



int main() {

    Dictionary dict;
    CacheManager cachemanager;
    KeyRecommander keyrecommander(dict, cachemanager);
    WebPageSearcher seacher;
    DataBag data;
    data.keyRecommanderPtr = &keyrecommander;
    data.webPageSearcherPtr = &seacher;
    EchoServer server(&data, 8888);
    server.start();

    

    return 0;
}
