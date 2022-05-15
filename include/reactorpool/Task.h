#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "/home/mikezhou/codes/SearchEnginerend/include/DataBag.h"

#include <functional>

using Task = std::function<void(DataBag *dataPtr)>;

#endif