#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/PagePreProcessor.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/SplitTool.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/tinyxml2.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/utf8.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
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
using std::log2;
using std::ofstream;
using std::ostringstream;
using std::unordered_set;

PagePreProcessor::PagePreProcessor() {}

void PagePreProcessor::invertProcess() {
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
    ifs.close();

    XMLDocument xml;
    if (xml.LoadFile("/home/mikezhou/codes/SearchEnginerend/data/ripepage.dat") != XML_SUCCESS) {
        cerr << "open xml fail" << endl;
        return;
    }

    XMLElement* doc = xml.RootElement();
    int docCnt = 0;
    while (doc) {
        XMLElement* docid = doc->FirstChildElement("docid");
        if (docid == nullptr) {
            doc = doc->NextSiblingElement();
            continue;
        }

        int id = std::stoi(docid->GetText());
        XMLElement* description = docid->NextSiblingElement("description");
        // cout << "1" <<endl;
        string content;
        if(description->GetText() != nullptr){
            // cout << "3" << endl;
            // printf("p = %p\n", description->GetText());
            content = string(description->GetText());
        }else{
            // cout << "4" << endl;
            doc = doc->NextSiblingElement();
            continue;
        }
        
        // cout << "2" << endl;
        // cout << "contentSz = " << content.size() << endl;
        if (content.size() != 0) {
            // cutWords = _cuttor->cut(content);
            jieba.Cut(content, cutWords, true);
            size_t curPos = 0;
            for (int i = 0; i < cutWords.size(); ++i) {
                if (stopWords.find(cutWords[i]) != stopWords.end() ||
                    isalnum(cutWords[i][0]) || isspace(cutWords[i][0])) {
                    curPos += cutWords[i].size();
                    continue;
                }
                // cout << cutWords[i] << endl;
                if (_docWords[id].find(cutWords[i]) == _docWords[id].end()) {
                    ++_wordsFreq[cutWords[i]];
                    _docWords[id].insert(cutWords[i]);
                }
                ++_docFreq[cutWords[i]][id].first;
                _docFreq[cutWords[i]][id].second = curPos;
                curPos += cutWords[i].size();
            }
            cutWords.clear();
        }


        doc = doc->NextSiblingElement();
        ++docCnt;

    }


    // ofstream ofs("dump.txt");
    // for (auto it = _wordsFreq.begin(); it != _wordsFreq.end(); ++it) {
    //     ofs << it->first << " " << it->second << "\n";
    // }
    // ofs.close();

    int N = docCnt;
    for (auto it1 = _docWords.begin(); it1 != _docWords.end(); ++it1) {
        int curId = it1->first;
        unordered_map<string, pair<double, int>> curW;
        double sumW = 0.0;
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            int TF = _docFreq[*it2][curId].first;
            int DF = _wordsFreq[*it2];
            double IDF = log2((double)N / (DF + 1));
            curW[*it2].first = TF * IDF;
            sumW += (TF * IDF) * (TF * IDF);

            curW[*it2].second = _docFreq[*it2][curId].second;
        }

        for (auto it3 = curW.begin(); it3 != curW.end(); ++it3) {
            _invertIndexTable[it3->first].insert(
                {curId, {it3->second.first / sqrt(sumW), it3->second.second}});
        }
    }
}

void PagePreProcessor::store() {
    ofstream ofs("/home/mikezhou/codes/SearchEnginerend/data/invertIndex.dat");
    for (auto it = _invertIndexTable.begin(); it != _invertIndexTable.end();
         ++it) {
        ofs << it->first << " ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            ofs << it2->first << " " << it2->second.first << " "
                << it2->second.second << " ,";
        }
        ofs << "\n";
    }
    ofs.close();
}

void PagePreProcessor::dump() {
    ofstream ofs("/home/mikezhou/codes/SearchEnginerend/src/pageDump/dump.txt");

    // for(auto it = _docWords.begin(); it != _docWords.end(); ++it){
    //     ofs << it->first << ":";
    //     for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
    //         ofs << *it2 << " ";
    //     }
    //     ofs << endl;
    // }

    for (auto it = _invertIndexTable.begin(); it != _invertIndexTable.end();
         ++it) {
        ofs << it->first << ":";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            ofs << it2->first << " ";
        }
        ofs << endl;
    }

    ofs.close();
}