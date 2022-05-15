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

    map<string, string>& getConfigMap(); //��ȡ�����ļ�����
    string getConfigValue(const string&); //��ȡ�����ļ�����
    unordered_set<string>& getStopWordList(); //��ȡͣ�ôʼ���

    static void init();
    static void destroy();

private:
    Configuration();
    pair<string, string> makePair(const string& Line);
    void configMapInit(const string& filename);
    void stopWordListInit();

private:
    static string _filepath; //�����ļ�·��
    //��һ��string ������Ҫȡ���������� �ڶ���string�����õľ�������
    //eg : "xml"--->"���xml�ļ���Ŀ¼��·��"
    //     "offset"--->"newoffset.lib"
    //     "page"--->"newpage.lib"
    //     "dict"--->"dict.dat"
    //     "index"--->"dictindx.dat"
    //     "stop"--->"ͣ�ô��ļ����·��"
    //     "ip" ---> "127.0.0.1"
    //     "port"--->"8023"
    map<string, string> _configMap; //�����ļ�����

    unordered_set<string> _stopWordList; //ͣ�ôʼ�

    static Configuration* _pInstance;
    static pthread_once_t _once;
};
#endif