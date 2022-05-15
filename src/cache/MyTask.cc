#include "/home/mikezhou/codes/SearchEnginerend/include/cache/MyTask.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/DataBag.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/KeyRecommander.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.h"

#include "/home/mikezhou/codes/SearchEnginerend/include/json.hpp"

#include <iostream>
#include <sstream>

#include <string.h>

using std::cout;
using std::endl;
using nlohmann::json;

MyTask::MyTask(const string& request, const TcpConnectionPtr& conn)
    : _request(request), _conn(conn) {}

MyTask::~MyTask() {}

void MyTask::process(DataBag* dataPtr) {
    cout << "SubThread " << dataPtr->ThreadIdx << " process" << endl;
    _request.pop_back();

    parse();

    if (operatorId == 1) {
        KeyRecommander& kr = *dataPtr->keyRecommanderPtr;
        vector<string> res;
        // cout << _request << endl;
        kr.doQuery(msgContent, dataPtr->ThreadIdx, res);

        json j;
        j["key"] = msgContent;
        for(int i = 0; i < res.size(); ++i){
            j["values"].push_back(res[i]);
        }

        std::ostringstream oss;
        oss << j;

        string response(oss.str());

        // cout << "response = " << response << endl;

        _conn->sendInLoop(response);
    }else if(operatorId == 2){
        WebPageSearcher& searcher = *dataPtr->webPageSearcherPtr;
        // cout << _request << endl;

        string response;

        searcher.doQuery(msgContent, response);

        // string response = _request;
        // string response;

        // for (int i = 0; i < res.size(); ++i) {
        //     response += res[i] + " ";
        // }

        // cout << "response = " << response << endl;

        _conn->sendInLoop(response);
    }else if(operatorId == 3){
        vector<string> keys;
        KeyRecommander& kr = *dataPtr->keyRecommanderPtr;
        kr.getRandDict(keys);

        json j;
        for(int i = 0; i < keys.size(); ++i){
            j["keys"].push_back(keys[i]);
        }

        std::ostringstream oss;
        oss << j;

        string response(oss.str());

        // cout << "response = " << response << endl;

        _conn->sendInLoop(response);
    }else if(operatorId = 4){
        vector<string> keys;
        KeyRecommander& kr = *dataPtr->keyRecommanderPtr;
        kr.getSolitaireKeys(msgContent,keys);
        
        cout << "keysSz = " << keys.size() <<endl;
        for(int i = 0; i < keys.size(); ++i){
            cout << keys[i] << endl;
        }

        json j;
        for(int i = 0; i < keys.size(); ++i){
            j["keys"].push_back(keys[i]);
        }

        std::ostringstream oss;
        oss << j;

        string response(oss.str());

        // cout << "response = " << response << endl;

        _conn->sendInLoop(response);
    }
}

void MyTask::parse() {

    char request[4096] = {0};
    memcpy(request, _request.c_str(), _request.size());
    cout << _request.size() << endl;

    char buf[4096] = {0};
    string tmp;
    int idx = 0;
    for (int i = 0; i < 4; ++i) {
        buf[i] = request[idx++];
    }
    memcpy(&msgLen, buf, 4);
    memset(buf, 0, sizeof(buf));

    for (int i = 0; i < 4; ++i) {
        buf[i] = request[idx++];
    }
    memcpy(&operatorId, buf, 4);
    memset(buf, 0, sizeof(buf));

    for (int i = 0; i < msgLen; ++i) {
        buf[i] = request[idx++];
    }

    msgContent = buf;

    cout << "msgLen:" << msgLen << endl;
    cout << "operatorId:" << operatorId << endl;
    cout << "msgContent:" << msgContent << endl;
}