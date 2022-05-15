#ifndef __MINDISTANCE_H__
#define __MINDISTANCE_H__

#include <string>

class MinEditDist
{
public:
	MinEditDist(){}
	int editDistance(const std::string &lhs, const std::string &rhs);

private:
	int nBytesCode(const char ch);
	int length(const string & str);
	int triple_min(const int &a, const int &b, const int &c);
};

 
int MinEditDist::nBytesCode(const char ch)
{
	int len = 1;
	if(ch & 1 << 7) {
		//多字节的字符
		for(int idx = 0; idx < 6; idx++) {
			if(ch & (1 << (6 - idx))) {
				++len;
			}else {
				return len;
			}
		}

	} else {
		//1个字节的字符
		return len;
	}
}

//求取一个字符串的字符长度
int MinEditDist::length(const string & str)
{
	int len = 0;
	for(size_t idx = 0; idx < str.size(); ++idx) {
		int bytes = nBytesCode(str[idx]);
		idx += (bytes - 1);
		++len;
	}
	return len;
}

// 0xxx xxxx
//&1000 0000
// 0000 0000 

int  MinEditDist::triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int MinEditDist::editDistance(const std::string & lhs, const std::string &rhs)
{//计算最小编辑距离-包括处理中英文
	size_t lhs_len = length(lhs);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);
		sublhs = lhs.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];
}

#endif