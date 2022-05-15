#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/tinyxml2.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;

using namespace tinyxml2;
using std::find;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::vector;

WebPageSearcher::WebPageSearcher() {
    initInvertIndex();
    initOffset();
}

void WebPageSearcher::initInvertIndex() {
    cout << "initialize invertIndex" << endl;
    ifstream ifs("/home/mikezhou/codes/SearchEnginerend/data/invertIndex.dat");
    // ifstream ifs("invertIndex.dat");
    string line;
    while (getline(ifs, line)) {
        if (line.size() == 0)
            break;
        bool keyFlag = false;
        bool idFlag = false;
        bool wFlag = false;
        bool posFlag = false;

        string key;
        int id = 0;
        double w = 0;
        int pos = 0;
        string tmp;
        for (int i = 0; i < line.size(); ++i) {
            // cout << line << endl;
            // cout << line.size() <<endl;
            if (line[i] == ' ') {
                if (!keyFlag) {
                    key = tmp;
                    tmp.clear();
                    keyFlag = true;
                    continue;
                }
                if (!idFlag) {
                    id = std::stoi(tmp);
                    tmp.clear();
                    idFlag = true;
                    continue;
                }
                if (!wFlag) {
                    w = std::stod(tmp);
                    tmp.clear();
                    wFlag = true;
                    continue;
                }
                if (!pos) {
                    pos = std::stoi(tmp);
                    tmp.clear();
                    posFlag = true;
                    break;
                }
            }
            tmp += line[i];
        }

        _invertIndexTable[key].insert({id, {w, pos}});
    }

    ifs.close();
}

void WebPageSearcher::initOffset() {
    cout << "initialize offset" << endl;
    ifstream ifs("/home/mikezhou/codes/SearchEnginerend/data/offset.dat");
    string line;
    while (getline(ifs, line)) {
        if (line.size() == 0)
            break;
        bool idFlag = false;
        bool begFlag = false;
        bool endFlag = false;

        size_t id = 0;
        size_t begPos = 0;
        size_t endPos = 0;
        string tmp;
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == ' ') {
                if (!idFlag) {
                    id = std::stoi(tmp);
                    tmp.clear();
                    idFlag = true;
                    continue;
                }
                if (!begFlag) {
                    begPos = std::stoi(tmp);
                    tmp.clear();
                    begFlag = true;
                    continue;
                }
                if (!endFlag) {
                    endPos = std::stod(tmp);
                    tmp.clear();
                    endFlag = true;
                    break;
                }
            }
            tmp += line[i];
        }

        _offset[id] = {begPos, endPos};
    }

    ifs.close();
}

void WebPageSearcher::doQuery(string key, string &res) {
    cout << key << endl;
    vector<string> keys;
    string tmp;
    for (int i = 0; i < key.size(); ++i) {
        if (key[i] != ' ') {
            tmp += key[i];
        } else {
            keys.push_back(tmp);
            tmp.clear();
        }
    }
    keys.push_back(tmp);

    cout << "keys size: " << keys.size() << endl;

    vector<int> calcId;
    if (keys.size() == 0 || keys.size() == 1) {
        singleQuery(key, calcId);
    } else {
        multiQuery(keys, calcId);
    }

    set<WebPage> webPageSet;

    simCalc(keys, calcId, webPageSet);

    showRes(keys, webPageSet);
}

void WebPageSearcher::simCalc(vector<string>& keys,
                              vector<int>& calcId,
                              set<WebPage>& webPageSet) {
    if (keys.size() > 1) {
        vector<double> wArr(keys.size());
        double w = std::log2((double)1 / 1 + 1);
        for (int i = 0; i < wArr.size(); ++i) {
            wArr[i] = w / sqrt((w * w) * keys.size());
        }

        for (int i = 0; i < calcId.size(); ++i) {
            vector<double> obwArr;
            for (int j = 0; j < keys.size(); ++j) {
                for (auto it = _invertIndexTable[keys[j]].begin();
                     it != _invertIndexTable[keys[j]].end(); ++it) {
                    if (it->first == calcId[i]) {
                        obwArr.push_back(it->second.first);
                    }
                }
            }

            double XY = 0.0;
            double modX = 0.0;
            double modY = 0.0;

            for (int i = 0; i < wArr.size(); ++i) {
                XY += wArr[i] * obwArr[i];
                modX += wArr[i] * wArr[i];
                modY += obwArr[i] * obwArr[i];
            }

            modX = sqrt(modX);
            modY = sqrt(modY);

            double cos = XY / (modX * modY);

            getWebPageSet(calcId[i], cos, webPageSet);
        }

    } else {
        auto& pageSet = _invertIndexTable[keys[0]];
        for (auto it = pageSet.begin(); it != pageSet.end(); ++it) {
            getWebPageSet(it->first, it->second.first, webPageSet);
        }
    }
}

void WebPageSearcher::getWebPageSet(int id,
                                    double cos,
                                    set<WebPage>& webPageSet) {
    ifstream ifs("/home/mikezhou/codes/SearchEnginerend/data/ripepage.dat");
    ifs.seekg(_offset[id].first, std::ios::beg);
    size_t readNum = _offset[id].second - _offset[id].first;
    char buf[65536] = {0};
    ifs.read(buf, readNum);
    string curPage;
    XMLDocument xml;
    WebPage page;
    xml.Parse(buf);
    XMLElement* doc = xml.RootElement();
    XMLElement* item = doc->FirstChildElement("docid");
    page.id = std::stoi(item->GetText());

    item = item->NextSiblingElement("title");
    page.title = item->GetText();

    item = item->NextSiblingElement("link");
    page.link = item->GetText();

    item = item->NextSiblingElement("description");
    page.description = item->GetText();

    page.cos = cos;

    webPageSet.insert(page);

    ifs.close();
}

void WebPageSearcher::singleQuery(string key, vector<int>& retId) {
    cout << "single key search" << endl;
    for (auto it = _invertIndexTable[key].begin();
         it != _invertIndexTable[key].end(); ++it) {
        retId.push_back(it->first);
    }
}

void WebPageSearcher::multiQuery(const vector<string>& keys,
                                 vector<int>& retId) {
    cout << "multi keys search" << endl;
    vector<int> firstKeyId;
    for (auto it = _invertIndexTable[keys[0]].begin();
         it != _invertIndexTable[keys[0]].end(); ++it) {
        firstKeyId.push_back(it->first);
    }

    for (int i = 1; i < keys.size(); ++i) {
        for (auto it = _invertIndexTable[keys[i]].begin();
             it != _invertIndexTable[keys[i]].end(); ++it) {
            vector<int>::iterator it2;
            if ((it2 = find(firstKeyId.begin(), firstKeyId.end(), it->first)) !=
                firstKeyId.end()) {
                retId.push_back(*it2);
            }
        }
    }

    // for(int i = 0; i < retId.size(); ++i){
    //     cout << retId[i] << " ";
    // }
    // cout << endl;
}

void WebPageSearcher::showRes(const vector<string>& keys,
                              const set<WebPage>& webPageSet) {
    for (auto it = webPageSet.begin(); it != webPageSet.end(); ++it) {
        cout << it->id << "\n" << it->title << "\n" << it->link << "\n";

        for (int _i = 0; _i < keys.size(); ++_i) {
            int idx = 0;
            auto& _set = _invertIndexTable[keys[_i]];
            for (auto it2 = _set.begin(); it2 != _set.end(); ++it2) {
                if (it2->first == it->id) {
                    idx += it2->second.second;
                    break;
                }
            }

            bool isbegin = (idx - 30) < 0;
            bool isend = (idx + 30) >= it->description.size();
            idx = (idx - 30) < 0 ? 0 : idx;
            idx = (idx + 30) >= it->description.size() ? it->description.size()
                                                       : idx;


            cout << "...";
            int i = 60;
            if (isbegin) {
                while (i--) {
                    cout << it->description[idx++];
                }
            } else if (isend) {
                idx -= 60;
                while (i--) {
                    cout << it->description[idx++];
                }
            } else {
                idx -= 30;
                while (i--) {
                    cout << it->description[idx++];
                }
            }

            cout << "...";
        }
        cout << endl;
    }
}