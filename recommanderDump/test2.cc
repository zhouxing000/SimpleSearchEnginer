// #include <string.h>
// #include <algorithm>
// #include <fstream>
// #include <iostream>
// #include <iterator>
// #include <string>
// #include <vector>
// #include <sstream>
// #include <map>
// #include <set>
// #include "../include/utf8.h"
// using std::cin;
// using std::cout;
// using std::endl;
// using std::string;
// using std::vector;
// using std::map;
// using std::set;
// using std::ostringstream;
// int main() {

//     map<string, set<int>> _index;
//     int cnt = 0;
//     string str = "今天天气真好";
//         for(auto it1 = str.begin(), it2 = str.begin(); it2 != str.end();){
//             utf8::next(it2, str.end());
//             ostringstream oss;
//             while(it1 < it2){
//                 oss << *it1;
//                 ++it1;
//             }
//             _index[oss.str()].insert(cnt++);
//         }

//     return 0;
// }
