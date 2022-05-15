#include "/home/mikezhou/codes/SearchEnginerend/include/DictProducer.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/SplitTool.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/utf8.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/tinyxml2.h"

#include <dirent.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_set>

#include "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/include/cppjieba/Jieba.hpp"

const char* const DICT_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/stop_words.utf8";

using namespace tinyxml2;
using std::cerr;
using std::cout;
using std::endl;
using std::ostringstream;
using std::regex;
using std::unordered_set;

DictProducer::DictProducer(string fileDir, SplitTool* splitToolptr)
    : _dict(new vector<pair<string, int>>),
      _index(new map<string, set<int>>),
      _cuttor(splitToolptr) {
    initFileName(fileDir);
}

void DictProducer::initFileName(string fileDir) {
    DIR* dirp = opendir(fileDir.c_str());
    dirent* pdirent;
    string dirName;
    while ((pdirent = readdir(dirp)) != nullptr) {
        dirName = pdirent->d_name;
        if (dirName != "." && dirName != "..") {
            _files.push_back(dirName);
        }
    }
    closedir(dirp);
}

void DictProducer::buildCnDict() {
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH,
                          STOP_WORD_PATH);
    vector<string> cutWords;
    vector<cppjieba::Word> jiebawords;

    unordered_set<string> stopWords;
    std::ifstream ifs("/home/mikezhou/codes/SearchEnginerend/include/stop_words_zh.txt");
    string tmpStr;
    while (getline(ifs, tmpStr)) {
        tmpStr.pop_back();
        stopWords.insert(tmpStr);
    }
    // cout << stopWords.size() << endl;
    ifs.close();

    map<string, int> _map;
    for (int i = 0; i < _files.size(); ++i) {
        cout << _files[i] << endl;
        XMLDocument xml;
         string fileName = "/home/mikezhou/codes/SearchEnginerend/material/" + _files[i];
        //string fileName = "../m/" + _files[i];
        if (xml.LoadFile(fileName.c_str()) != XML_SUCCESS) {
            cerr << "open xml fail" << endl;
            return;
        }
        XMLElement* rss = xml.RootElement();
        XMLElement* channel = rss->FirstChildElement();
        XMLElement* item = channel->FirstChildElement("item");

        regex reg("<\\/?.+?\\/?>");
        string contextTitle;
        string contextDescription;
        while (item) {
            XMLElement* itemChild = item->FirstChildElement("title");
            if (itemChild == nullptr) {
                item = item->NextSiblingElement();
                continue;
            }

            contextTitle = regex_replace(itemChild->GetText(), reg, "");

            // cutWords = _cuttor->cut(contextTitle);
            jieba.Cut(contextTitle, cutWords, true);
            for (int i = 0; i < cutWords.size(); ++i) {
                if (stopWords.find(cutWords[i]) != stopWords.end() ||
                    isalnum(cutWords[i][0]) || isspace(cutWords[i][0])) {
                    continue;
                }
                cout << cutWords[i] << endl;
                ++_map[cutWords[i]];
            }
            cutWords.clear();

            itemChild = itemChild->NextSiblingElement("description");
            contextDescription = regex_replace(itemChild->GetText(), reg, "");

            // cutWords = _cuttor->cut(contextDescription);
            jieba.Cut(contextDescription, cutWords, true);
            for (int i = 0; i < cutWords.size(); ++i) {
                if (stopWords.find(cutWords[i]) != stopWords.end() ||
                    isalnum(cutWords[i][0]) || isspace(cutWords[i][0])) {
                    continue;
                }

                cout << cutWords[i] << endl;
                ++_map[cutWords[i]];
            }

            item = item->NextSiblingElement();
        }

        // createIndex();

        // store();

        // _map.clear();
        // _dict->clear();
        // _index->clear();

    }
    for (auto it = _map.begin(); it != _map.end(); ++it) {
            _dict->push_back(*it);
        }
}

void DictProducer::createIndex() {
    for (size_t i = 0; i < _dict->size(); ++i) {
        for (auto it1 = (*_dict)[i].first.begin(),
                  it2 = (*_dict)[i].first.begin();
             it2 != (*_dict)[i].first.end();) {
            utf8::next(it2, (*_dict)[i].first.end());
            ostringstream oss;
            while (it1 < it2) {
                oss << *it1;
                ++it1;
            }
            cout << oss.str() << ":";
            (*_index)[oss.str()].insert(i);
            // for(auto it = _index[oss.str()].begin(); it !=
            // _index[oss.str()].end(); ++it){
            //     cout << *it << endl;
            // }
            // cout << endl;
        }
    }
}

void DictProducer::store() {
    cout << "store dict.dat" << endl;
    std::ofstream ofs1("/home/mikezhou/codes/SearchEnginerend/data/dict.dat");
    for (auto it = _dict->begin(); it != _dict->end(); ++it) {
        ofs1 << it->first << " " << it->second << "\n";
    }
    ofs1.close();

    cout << "store dictIndex.dat" << endl;
    std::ofstream ofs2("/home/mikezhou/codes/SearchEnginerend/data/dictIndex.dat");
    for (auto it = _index->begin(); it != _index->end(); ++it) {
        ofs2 << it->first << " ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            ofs2 << *it2 << " ";
        }
        ofs2 << "\n";
    }
    ofs2.close();
}