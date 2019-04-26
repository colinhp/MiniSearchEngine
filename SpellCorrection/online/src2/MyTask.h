 ///
 /// @file    MyTask.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-26 10:36:29
 ///
 
#ifndef __MYTASK_H__
#define __MYTASK_H__ 

#include "Dictionary.h"
#include "TcpConnection.h"

#include <vector>
#include <string>
#include <queue>
#include <utility>
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;

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
	MyTask(string& query, TcpConnectionPtr conn, Dictionary myDict);
	void findQuery();//查询

private:
	int minOfThree(int a,int b,int c);
	int editDistance(const string& word);

private:
	string query_;
	TcpConnectionPtr conn_;
	Dictionary myDict_;
	Result res_;
	priority_queue<Result,vector<Result>,Compare> resQue_;
};

}//end of namespace kk

#endif
