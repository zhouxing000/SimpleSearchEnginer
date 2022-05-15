#include "/home/mikezhou/codes/SearchEnginerend/include/pageDump/SplitTool.hpp"
#include "/home/mikezhou/codes/SearchEnginerend/include/DictProducer.hpp"

#include <iostream>

using std::endl;
using std::cout;

int main()
{
    SplitTool *psp = new SplitTool();
     DictProducer *pdp = new DictProducer("/home/mikezhou/codes/SearchEnginerend/material", psp);
    pdp->buildCnDict();
    pdp->createIndex();
    pdp->store();
    return 0;
}

