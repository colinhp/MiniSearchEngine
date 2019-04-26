 ///
 /// @file    MyTask.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-26 11:23:03
 ///
 
#include "MyTask.h"
#include "Cache.h"
#include "CacheManager.h"
#include "CutByCharacter.h"//for nBytesCode() & length()

#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;
using std::set;

using namespace kk;

MyTask::MyTask(string& query, TcpConnectionPtr conn, Dictionary myDict)
: query_(query)
, conn_(conn)
, myDict_(myDict)
{}

void MyTask::findQuery()
{
	cout<<"query: "<<query_<<endl;//测试
	//每次查询前,先从本线程的内存cache中查找
	Cache& currCache = CacheManager::getCache(pthread_self());
	cout<<"current pid = "<<pthread_self()<<endl;
	currCache.show();

	if(currCache.hasQueryResult(query_)){//如果命中cache,直接从缓存文件中返回结果
		cout<<"find result in cache"<<endl;
		conn_->sendInLoop(currCache.getResult(query_));
	}	
	else//如果没有命中Cache,记得最后要将最新结果写入cache
	{
		auto indexMap = myDict_.getIndexMap();
		auto dictionary = myDict_.getDict();
	
		set<string> strSet;//单词过滤
		//将得到的查询单词拆分为单个字符，并依据每个字符找到其索引
		for(size_t i = 0; i != query_.size(); ){
			cout<<"length(query_) = "<<length(query_)<<endl;
			size_t nbyte = nBytesCode(query_[i]);
			cout<<"nbyte = "<<nbyte<<endl;
			string ch = query_.substr(i,nbyte);//出现错误
			i += nbyte;

			cout<<"---key_value: "<<ch<<endl;
				
			auto iter = indexMap.find(ch);//查找当前字符
	
			if(iter != indexMap.end() ){//如果找到
				auto& retSet = iter->second;
				for(auto it = retSet.begin();it != retSet.end(); ++it){//遍历该字母对应的索引单词
	
					res_.word_ = dictionary[*it].first;//单词
					//cout<<"下标-单词-词频: "<<*it<<" "<<dictionary[*it].first<<" "<<cnt<<endl;	//出错

					if(strSet.end() == strSet.find(res_.word_)){//如果该单词是首次出现,对其进行处理
						strSet.insert(res_.word_);
						res_.count_ = dictionary[*it].second;//词频
						res_.distance_ = editDistance(res_.word_);//当前单词与查询单词的最小编辑距离
						if(res_.distance_ <= 3){
							resQue_.push(res_);//把当前结果压入优先队列	
						}
					}else{
						continue;
					}
				}
			}else{
				cout<<"索引表不存在该键值"<<endl;
			}
		}//end of for
		


		if(!resQue_.empty()){//如果对端输入一串乱码,那么优先队列肯定为空
			const string& queryRes = resQue_.top().word_;
			conn_->sendInLoop(queryRes);//把优先级队列里的第一个元素发给对端
			currCache.addElement(query_,queryRes);//把查询词以及与其匹配的结果添加至内存cache
		}else{
			conn_->sendInLoop("sorry,can't find!");
		}
		//测试
		int cnt = 5;//打印优先队列中的前五个元素(如果有>=5个元素的话)
		while(!resQue_.empty() && cnt > 0){
			auto res = resQue_.top();
			cout<<res.word_<<"-"<<res.count_<<"-"<<res.distance_<<endl;//打印“单词-词频-编辑距离”
			resQue_.pop();
			--cnt;
		}




	}//没有命中else
}

int MyTask::minOfThree(int a, int b, int c)
{
	return (a<b)?((a<c)?a:c):((b<c)?b:c);
}

//求最短编辑距离
int MyTask::editDistance(const string& rhs)
{
//	size_t lenRhs = rhs.size();//中文编码不再适用
//	size_t lenLhs = query_.size();

	//中英文编码统一
	size_t lenRhs = length(rhs);
	size_t lenLhs = length(query_);

	int editDist[lenLhs+1][lenRhs+1];
	
	for(size_t i = 0; i<=lenLhs; ++i){
		editDist[i][0] = i;
	}

	for(size_t i = 0; i<=lenRhs; ++i){
		editDist[0][i] = i;
	}

	string subLhs, subRhs;

	for(size_t i = 1, subi = 0; i <= lenLhs; ++i){
		size_t nbyte = nBytesCode(query_[subi]);
		subLhs = query_.substr(subi,nbyte);//截取子串
		subi += nbyte;

		for(size_t j = 1, subj = 0; j <= lenRhs; ++j){
			nbyte = nBytesCode(rhs[subj]);
			subRhs = rhs.substr(subj,nbyte);//截取子串
			subj += nbyte;

			if(subLhs == subRhs){
				editDist[i][j] = editDist[i-1][j-1];
			}else{
				editDist[i][j] = minOfThree(editDist[i-1][j]+1,
											editDist[i][j-1]+1,
											editDist[i-1][j-1]+1);
			}
		}
	}//for
	return editDist[lenLhs][lenRhs];
}
