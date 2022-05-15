#include "/home/mikezhou/codes/SearchEnginerend/include/Configuration.h"

#include <pthread.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::vector;

//初始化静态数据成员
//饱汗模式
Configuration* Configuration::_pInstance = nullptr;
pthread_once_t Configuration::_once = PTHREAD_ONCE_INIT;
string Configuration::_filepath = "/home/mikezhou/codes/SearchEnginerend/conf/cat.cfg";

Configuration* Configuration::getInstance()
{
    pthread_once(&_once, init);

    return _pInstance;
}

void Configuration::init()
{
    _pInstance = new Configuration();
    atexit(destroy);
}

void Configuration::destroy()
{
    if (_pInstance) {
        delete _pInstance;
        _pInstance = nullptr;
    }
}

Configuration::Configuration()
{
    //初始化_configMap
    configMapInit(Configuration::_filepath);

    //初始化_stopWordList
    stopWordListInit();
}

pair<string, string> Configuration::makePair(const string& Line)
{
    istringstream iss(Line);
    string word1, word2;
    iss >> word1;
    iss >> word2;
    pair<string, string> onePair(word1, word2);
    return onePair;
}

map<string, string>& Configuration::getConfigMap()
{
    return _configMap;
}

unordered_set<string>& Configuration::getStopWordList()
{
    return _stopWordList;
}

void Configuration::configMapInit(const string& filename)
{
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cerr << "ConfigurationFile Open error" << endl;
        return;
    }

    ifs.seekg(std::ios_base::beg);
    string Line;
    while (getline(ifs, Line)) {
        _configMap.insert(makePair(Line));
    }
    ifs.close();
}

void Configuration::stopWordListInit()
{
    //获取路径
    string StopWordFilePath(_configMap["stop"]);

    ifstream sifs(StopWordFilePath);
    if (!sifs.is_open()) {
        cerr << "StopWordFile Open error" << endl;
        return;
    }

    sifs.seekg(std::ios_base::beg);
    string stopWord;
    while (getline(sifs, stopWord)) {
        _stopWordList.insert(stopWord);
    }
    sifs.close();
}

