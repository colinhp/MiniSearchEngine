 ///
 /// @file    MyTask.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-26 10:36:29
 ///
 
#ifndef __MYTASK_H__
#define __MYTASK_H__ 

#include "TcpConnection.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <set>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::map;
using std::set;

namespace kk
{
struct Result
{
	string word_;
	int count_;
	int distance_;//min edit distance
};

struct Compare
{
	bool operator()(const Result& lhs,const Result& rhs)
	{
		if(lhs.distance_ > rhs.distance_)
			return true;
		else if(lhs.distance_ == rhs.distance_
		    	&& lhs.count_ < rhs.count_)
			return true;
		else if(lhs.distance_ == rhs.distance_
				&& lhs.count_ == rhs.count_
				&& lhs.word_ < rhs.word_)
			return true;
		else
			return false;
	}
};

class MyTask
{
public:
	MyTask(string& query, TcpConnectionPtr conn);
	void init();
	void findQuery();//查询
	int editDistance(const string& word);

private:
	int minOfThree(int a,int b,int c);
	string query_;
	TcpConnectionPtr conn_;
	Result res_;
	priority_queue<Result,vector<Result>,Compare> resQue_;
	vector<pair<string,int>> dictionary_;//用于初始化
	map<string,set<int>> indexMap_;//用于初始化	
//	map<string,string> confMap_;
};

}//end of namespace kk

#endif
