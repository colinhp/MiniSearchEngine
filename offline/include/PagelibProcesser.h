 ///
 /// @file    PagelibProcesser.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 16:19:09
 ///

#ifndef __PAGELIBPROCESSER_H__
#define __PAGELIBPROCESSER_H__
#include "WebPage.h"
#include "WordSegmentation.h"
#include "Configuration.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::unordered_map;
using std::set;

namespace kk
{
class PagelibProcesser
{
public:
	PagelibProcesser(Configuration& conf);
	void doProcess();

private:
	void readFile();//从磁盘中读取网页库和网页偏移库
	void createInvertIndexTable();//创建倒排索引表
	void store();//将倒排索引表存入磁盘

private:
	Configuration conf_;
	WordSegmentation jieba_;
	vector<WebPage> pagelib_;
	unordered_map<int, pair<int, int>> offsetlib_;
	unordered_map<string, vector<pair<int, double>>> invertIndexTable_;//倒排索引
};

}//end of namespace kk
#endif
