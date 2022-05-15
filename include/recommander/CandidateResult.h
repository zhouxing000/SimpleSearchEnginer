#ifndef __CANDIDATERESULT_H__
#define __CANDIDATERESULT_H__

#include <string>

using std::string;

struct CandidateResult
{
    string _word;
    int _fre;
    int _dis;
    bool operator<(const CandidateResult &rhs) const{
        if(_dis != rhs._dis){
            return _dis > rhs._dis;
        }else{
            return _fre < rhs._fre;
        }
    }
};


#endif