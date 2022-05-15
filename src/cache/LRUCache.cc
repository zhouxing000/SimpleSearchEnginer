#include "/home/mikezhou/codes/SearchEnginerend/include/cache/LRUCache.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::ostringstream;


bool LRUCache::get(string key, vector<string> &value) {
    // cout << "key:" << key << endl;
    // cout << "size" << _umap.size() << endl;
    auto it = _umap.find(key);
    
    // unordered_map<string, list<shared_ptr<CacheNode>>::iterator>::iterator it;
    // for(it = _umap.begin(); it != _umap.end(); ++it){
    //     if((*it).first == key){
    //         break;
    //     }
    // }
    if (it != _umap.end()) {
        _lru.splice(_lru.begin(), _lru, it->second);
        value = (*(it->second))->value;
        return true;
    }
    return false;
}

void LRUCache::put(string key, vector<string> value) {
    auto it = _umap.find(key);
    if (it != _umap.end()) {
        _lru.splice(_lru.begin(), _lru, it->second);
        (*(it->second))->value = value;
        _umap[key] = _lru.begin();
        return;
    }
    _lru.push_front(make_unique<CacheNode>(key, value));
    _umap[key] = _lru.begin();
    if (_umap.size() > _capacity) {
        _umap.erase(_lru.back()->key);
        _lru.pop_back();
    }

    _pendingUpdateList.push_front(make_unique<CacheNode>(key, value));
}

void LRUCache::show(){
    for(auto it = _lru.begin(); it != _lru.end(); ++it){
        cout << (*it)->key << ":";
        for(auto it2 = (*it)->value.begin(); it2 != (*it)->value.end(); ++it2){
            cout << *it2 << " ";
        }
        cout << endl;
    }
}

void LRUCache::writeToFile(string filename){
    cout << "cache write data to file" << endl;
    ofstream ofs(filename);
    for(auto it1 = _lru.begin(); it1 != _lru.end(); ++it1){
        string str = (*it1)->key;
        // str.pop_back();
        ofs << str << ":";
        for(auto it2 = (*it1)->value.begin(); it2 != (*it1)->value.end(); ++it2){
            ofs << *it2 << " ";
        }
        ofs << "\n";
    }
    ofs.close();
}

void LRUCache::readFromFile(string filename){
    cout << "cache read data from file" << endl;
    ifstream ifs(filename);
    string line;
    while(getline(ifs, line)){
        bool flag = false;
        string key;
        vector<string> value;
        string str;
        for(int i = 0; i < line.size(); ++i){
            if(line[i] == ':'){
                flag = true;
                continue;
            }
            if(!flag){
                key += line[i];
            }else{
                if(line[i] == ' '){
                    value.push_back(str);
                    str.clear();
                    continue;
                }
                str += line[i];
            }
        }

        this->put(key, value);
    }
    ifs.close();
}

void LRUCache::updateP2M(LRUCache &updateOb){
    string key;
    vector<string> value;
    for (auto it1 = updateOb._pendingUpdateList.begin();
        it1 != updateOb._pendingUpdateList.end(); ++it1) {
        key = (*it1)->key;
        
        // for (auto it2 = (*it1)->value.begin(); it2 != (*it1)->value.end();
        //     ++it2) {
        //     value.push_back(*it2);
        // }
        value = (*it1)->value;
        this->put(key, value);
        key.clear();
        value.clear();
    }
    updateOb._pendingUpdateList.clear();
}

void LRUCache::updateM2M(LRUCache &updateOb){
    _lru.clear();
    _umap.clear();
    string key;
    vector<string> value;
    for (auto it1 = updateOb._lru.begin();it1 != updateOb._lru.end(); ++it1) {
        key = (*it1)->key;
        // for (auto it2 = (*it1)->value.begin(); it2 != (*it1)->value.end();
        //     ++it2) {
        //     value.push_back(*it2);
        // }
        value = (*it1)->value;
        this->put(key, value);
        _pendingUpdateList.clear();
        key.clear();
        value.clear();
    }
}