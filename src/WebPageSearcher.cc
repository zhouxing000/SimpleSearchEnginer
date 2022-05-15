#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/tinyxml2.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/json.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/Configuration.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;

using nlohmann::json;
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
    Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["invert"];
    ifstream ifs(str);
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
            if (line[i] == ',') {
                idFlag = false;
                wFlag = false;
                posFlag = false;
                continue;
            }
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
                if (!posFlag) {
                    pos = std::stoi(tmp);
                    tmp.clear();
                    posFlag = true;

                    _invertIndexTable[key].insert({id, {w, pos}});
                    continue;
                }
            }
            tmp += line[i];
        }
    }

    ifs.close();
}

void WebPageSearcher::initOffset() {
    cout << "initialize offset" << endl;
     Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["offset"];
    ifstream ifs(str);
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

void WebPageSearcher::doQuery(string key, string& res) {
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

    cout << "id num:" << calcId.size() << endl;

    set<WebPage> webPageSet;

    simCalc(keys, calcId, webPageSet);

    // showRes(keys, webPageSet);

    outRes(keys, webPageSet, res);
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
    //Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["page"];
    ifstream ifs(str);
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

void WebPageSearcher::outRes(const vector<string>& keys,
                             const set<WebPage>& webPageSet,
                             string& res) {
    ostringstream oss;
    // oss << '[';

    json jSum;

    for (auto it = webPageSet.begin(); it != webPageSet.end(); ++it) {
        // oss << it->id << "\n" << it->title << "\n" << it->link << "\n";
        json j;
        j["id"] = it->id;
        j["title"] = it->title;
        j["link"] = it->link;

        ostringstream oss1;

        for (int _i = 0; _i < keys.size(); ++_i) {
            size_t idx = 0;
            auto& _set = _invertIndexTable[keys[_i]];
            for (auto it2 = _set.begin(); it2 != _set.end(); ++it2) {
                if (it2->first == it->id) {
                    idx += it2->second.second;
                    break;
                }
            }

            // size_t idx_ch = 0;
            // size_t idx_ch_begin = 0;
            size_t idx_byte_begin = 0;
            // size_t idx_ch_end = 0;
            size_t idx_byte_end = 0;
            bool ch_begin_flag = false;
            bool ch_end_flag = false;

            for(size_t k = 0; k < it->description.size();){
                size_t n = nBytesCode(it->description[k]);
                // ++idx_ch;
                k += n;
                if(!ch_begin_flag && k > ((idx - 30) < 0 ? 0 : (idx - 30))){
                    idx_byte_begin = k;
                    // idx_ch_begin = idx_ch;
                    ch_begin_flag = true;
                }
                if(!ch_end_flag && k > (idx + 30)){
                    idx_byte_end = k;
                    // idx_ch_end = idx_ch;
                    ch_end_flag = true;
                    break;
                }

            }

            idx_byte_end =( idx_byte_end == 0 ? it->description.size() : idx_byte_end);

            for(int k = idx_byte_begin; k < idx_byte_end; ++k){
                oss1 << it->description[k];
            }

            // string str(oss1.str());

            // cout << str << endl;
            // cout << "strSz:" << str.size() << endl;

            // bool isbegin = (idx - 30) < 0;
            // bool isend = (idx + 30) >= it->description.size();
            // idx = (idx - 30) < 0 ? 0 : idx;
            // idx = (idx + 30) >= it->description.size() ? it->description.size()
            //                                            : idx;

            // // oss << "...";
            // // oss1 << "...";
            // int i = 60;
            // if (isbegin) {
            //     while (i--) {
            //         // oss << it->description[idx++];
            //         oss1 << it->description[idx++];
            //     }
            // } else if (isend) {
            //     idx -= 60;
            //     while (i--) {
            //         // oss << it->description[idx++];
            //         oss1 << it->description[idx++];
            //     }
            // } else {
            //     idx -= 30;
            //     while (i--) {
            //         // oss << it->description[idx++];
            //         oss1 << it->description[idx++];
            //     }
            // }

            // oss << "...";
            // oss1 << "...";

            j["description"].push_back(oss1.str());

        }
        // oss << endl;
        jSum.push_back(j);
    }

    // oss << ']';

    oss << jSum;

    res = oss.str();
}

size_t WebPageSearcher::nBytesCode(const char ch)
{
	if(ch & (1 << 7))  //1000 0000
	{				   //1xxx 0000
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  