#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/Dictionary.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/minDistance.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/Configuration.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/utf8.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <queue>

using std::ifstream;
using std::ostringstream;
using std::cout;
using std::endl;
using std::priority_queue;

Dictionary::Dictionary()
{
    initDict();
    initIndex();
}

void Dictionary::initDict(){
    cout << "initialize dictionary" << endl;
    //  Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap()["dict"];
    ifstream ifs(str);
    string line;
    while(getline(ifs, line)){
        string word;
        string fre;
        bool flag = false;
        for(int i = 0; i < line.size(); ++i){
            if(line[i] == ' '){
                flag = true;
                continue;
            }
            if(!flag){
                word += line[i];
            }else{
                fre += line[i];
            }
        }
        int _fre = stoi(fre);
        _dict.push_back({word, _fre});
        
    }
    ifs.close();
}

void Dictionary::initIndex(){
    cout << "initialize index" << endl;
    // Configuration *con= Configuration::getInstance();
    string str=Configuration::getInstance()->getConfigMap() ["index"];
    ifstream ifs(str);
    string line;
    while(getline(ifs, line)){
        string word;
        string num;
        bool flag = false;
        for(int i = 0; i < line.size(); ++i){
            if(!flag && line[i] == ' '){
                continue;
            }
            if(flag && line[i] == ' '){
                int _num = stoi(num);
                _index[word].insert(_num);
                num.clear();
            }
            if(!flag && line[i] >= '0' && line[i] <= '9'){
                flag = true;
            }
            if(!flag){
                word += line[i];
            }else{
                num += line[i];
            }
        }
    }
    ifs.close();
}

void Dictionary::show_dict(){
    for(auto it = _dict.begin(); it != _dict.end(); ++it){
        cout << it->first << ":" << it->second << endl;
    }
}

void Dictionary::show_index(){
    for(auto it1 = _index.begin(); it1 != _index.end(); ++it1){
        cout << it1->first << ":";
        for(auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2){
            cout << *it2 << " ";
        }
        cout << endl;
    }
}

vector<string> Dictionary::splitSought(string sought){
    vector<string> ret;
    for(auto it1 = sought.begin(), it2 = sought.begin(); it2 != sought.end();){
        utf8::next(it2, sought.end());
        ostringstream oss;
        while(it1 < it2){
            oss << *it1;
            ++it1;
        }
        // cout << oss.str() << endl;
        ret.push_back(oss.str());
        // cout << endl;
    }
    return ret;
}

vector<string> Dictionary::doQuery(const string &sought){
    vector<string> words = splitSought(sought);
    for(int i = 0; i < words.size(); ++i){
        cout << words[i] << "/";
    }
    cout << endl;
    priority_queue<CandidateResult> que;
    MinEditDist mindis;
    
    for(int i = 0; i < words.size(); ++i){
        // cout << words[i] << endl;
        CandidateResult candidateWord;
        set<int> _set = _index[words[i]];

        for(auto it = _set.begin(); it != _set.end(); ++it){
            candidateWord._word = _dict[*it].first;
            candidateWord._fre = _dict[*it].second;
            candidateWord._dis = mindis.editDistance(sought, candidateWord._word);
            que.push(candidateWord);
        }
    }

    vector<string> ret;
    int cnt = 5;
    while(cnt){
        if(que.empty()){
            break;
        }

        for(int i = 0; i < ret.size(); ++i){
            if(ret[i] == que.top()._word){
                que.pop();
            }
        }

        ret.push_back(que.top()._word);
        que.pop();
        --cnt;
    }

    return ret;
}


void Dictionary::getRandKeys(vector<string> &keys){
    ::srand(::clock());
    for(int i = 0; i < 5; ++i){
        int idx = ::rand() % _dict.size();

        keys.push_back(_dict[idx].first);

    }
}

void Dictionary::getSolitaireKeys(string &key, vector<string> &keys){
    int keyLen = length(key);
    int idx_ch_key = 0;
    string key_last_ch;
    for(int i = 0; i < key.size();){
        if(idx_ch_key == keyLen - 1){
            int ch_byte_len = nBytesCode(key[i]);
            for(int j = 0; j < ch_byte_len; ++j){
                key_last_ch.push_back(key[i++]);
            }
            break;
        }

        int n = nBytesCode(key[i]);
        ++idx_ch_key;
        i += n;
    }

    cout << "last_ch:" << key_last_ch << endl;

    for(int i = 0; i < _dict.size(); ++i){
        string cur_key = _dict[i].first;
        // cout << cur_key << endl;
        int idx = 0;
        for(; idx < key_last_ch.size(); ++idx){
            if(key_last_ch[idx] != cur_key[idx]){
                break;
            }
        }
        if(idx == key_last_ch.size()){
            keys.push_back(_dict[i].first);
        }
        if(keys.size() >= 5){
            break;
        }
    }
}

size_t Dictionary::nBytesCode(const char ch)
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

size_t Dictionary::length(const std::string &str)
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;
}