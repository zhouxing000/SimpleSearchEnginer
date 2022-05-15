#ifndef _CONFIGURATION_H__
#define _CONFIGURATION_H__

#include <map>
#include <string>
#include <unordered_set>

using std::map;
using std::pair;
using std::string;
using std::unordered_set;

class Configuration {
public:
    static Configuration* getInstance();

    map<string, string>& getConfigMap(); //获取配置文件内容
    string getConfigValue(const string&); //获取配置文件内容
    unordered_set<string>& getStopWordList(); //获取停用词集合

    static void init();
    static void destroy();

private:
    Configuration();
    pair<string, string> makePair(const string& Line);
    void configMapInit(const string& filename);
    void stopWordListInit();

private:
    static string _filepath; //配置文件路径
    //第一个string 代表想要取的配置类型 第二个string是配置的具体内容
    //eg : "xml"--->"存放xml文件的目录的路径"
    //     "offset"--->"newoffset.lib"
    //     "page"--->"newpage.lib"
    //     "dict"--->"dict.dat"
    //     "index"--->"dictindx.dat"
    //     "stop"--->"停用词文件存放路径"
    //     "ip" ---> "127.0.0.1"
    //     "port"--->"8023"
    map<string, string> _configMap; //配置文件内容

    unordered_set<string> _stopWordList; //停用词集

    static Configuration* _pInstance;
    static pthread_once_t _once;
};
#endif