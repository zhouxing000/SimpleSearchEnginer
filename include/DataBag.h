#ifndef __DATABAG_H__
#define __DATABAG_H__

#include "/home/mikezhou/codes/SearchEnginerend/include/recommander/KeyRecommander.h"
#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/WebPageSearcher.h"



struct DataBag
{
    int ThreadIdx;
    KeyRecommander *keyRecommanderPtr;
    WebPageSearcher *webPageSearcherPtr;
};

#endif