 ///
 /// @file    WordQuery.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-07 12:18:17
 ///
 
#ifndef __WORDQUERY_H__
#define __WORDQUERY_H__

#include "WebPage.h"
#include "Configuration.h"
#include "WordSegmentation.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <utility>
using std::vector;
using std::string;
using std::set;
using std::map;
using std::unordered_map;
using std::unordered_set;
using std::pair;

namespace kk
{
class WordQuery
{
public:
	WordQuery(Configuration& conf);
	string doQuery(const string& queryStr);//根据查询语句，返回json字符串
private:
	void loadLibrary();//从磁盘中加载网页库、偏移库和倒牌索引表
	vector<double> getWeightOfQuery(vector<string>& queryStr);
	bool executeQuery(const vector<string>& queryStr, vector<pair<int,vector<double>>>& resultVec);
	string createJson(vector<int>& docidVec, const vector<string>& queryKeyWords);
	string noAnswer();
private:
	WordSegmentation jieba_;//用于对查询语句的分词
	Configuration conf_;
	unordered_map<int,WebPage> pageLib_;//保存网页库
	unordered_map<int,pair<int,int>> offsetLib_;//保存偏移库
	unordered_map<string,set<pair<int, double>>> invertIndexTable_;//保存倒排索引表
	unordered_set<string> stopwords_;//用于过滤客户端查询语句
};

}//end of namespace kk

#endif
