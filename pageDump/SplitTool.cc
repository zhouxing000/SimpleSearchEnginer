#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/SplitTool.h"

#include "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/include/cppjieba/Jieba.hpp"

const char* const DICT_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "/home/mikezhou/codes/SearchEnginerend/lib/cppjieba/dict/stop_words.utf8";


vector<string> SplitTool::cut(string str){
    cppjieba::Jieba jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH,
        IDF_PATH,
        STOP_WORD_PATH);
    vector<string> words;
    vector<cppjieba::Word> jiebawords;
    jieba.Cut(str, words, true);
    return words;
}