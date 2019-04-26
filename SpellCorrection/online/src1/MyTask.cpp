 ///
 /// @file    MyTask.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-26 11:23:03
 ///
 
#include "MyTask.h"

#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

using namespace kk;

MyTask::MyTask(string& query, TcpConnectionPtr conn)
: query_(query)
, conn_(conn)
{}

//初始化,读取词典存放至vector<pair<string,int>> dictionary_ 
//       读取索引存放至map<string,set<int>> indexMap_
void MyTask::init()
{
//	string path_endict = confMap_["en_dict"];
	string path_endict = "/home/kkmjy/C++/SpellCorrection/data/endict.dat";//暂时这么写
	ifstream in(path_endict);
	if(!in){
		cout<<"Task open endict.dat error!"<<endl;
	}

	string line;
	string word,count;
	while(getline(in,line)){
		istringstream iss(line);
		iss >> word >> count;
		dictionary_.push_back(make_pair(word,stoi(count)));
	}

/*	//测试
	for(size_t i = 0; i != dictionary.size(); ++i){
		cout<<dictionary[i].first<<" "<<dictionary[i].second<<endl;
	}
*/
	in.close();

//	string path_index = confMap_["en_index"];
	string path_index = "/home/kkmjy/C++/SpellCorrection/data/endict.index";
	ifstream in2(path_index);
	if(!in2){
		cout<<"Task open endict.dat error!"<<endl;
	}
	string character,idx;
	while(getline(in2,line)){
		istringstream iss(line);
		iss >> character;
		while(iss >> idx){
			indexMap_[character].insert(stoi(idx));
		}
	}

	in2.close();

/*	//测试	
	for(auto iter = indexMap.begin(); iter != indexMap.end(); ++iter){
		cout<<iter->first<<" : ";
		auto retSet = iter->second;
		for(auto it = retSet.begin();it != retSet.end(); ++it){
			cout<<*it<<" ";
		}
		cout<<endl;
	}
*/
}

int MyTask::minOfThree(int a, int b, int c)
{
	return (a<b)?((a<c)?a:c):((b<c)?b:c);
}

//求最短编辑距离
int MyTask::editDistance(const string& rhs)
{
	size_t lenRhs = rhs.size();
	size_t lenLhs = query_.size();
	int editDist[lenLhs+1][lenRhs+1];
	
	for(size_t i = 0; i<=lenLhs; ++i){
		editDist[i][0] = i;
	}

	for(size_t i = 0; i<=lenRhs; ++i){
		editDist[0][i] = i;
	}

	for(size_t i = 1; i <= lenLhs; ++i){
		for(size_t j = 1; j <= lenRhs; ++j){
			if(query_[i-1] == rhs[j-1]){
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

void MyTask::findQuery()
{
	//
	//0. 先在缓存中进行查找
	//
	//
	//土鳖做法
	map<char,string> charToStr{{'a',"a"},{'b',"b"},{'c',"c"},{'d',"d"},{'e',"e"},{'f',"f"},{'g',"g"},{'h',"h"},
							   {'i',"i"},{'j',"j"},{'k',"k"},{'l',"l"},{'m',"m"},{'n',"n"},{'o',"o"},{'p',"p"},
	                           {'q',"q"},{'r',"r"},{'s',"s"},{'t',"t"},{'u',"u"},{'v',"v"},{'w',"w"},{'x',"x"},
							   {'y',"y"},{'z',"z"}};

	cout<<"query "<<query_<<endl;

	set<string> strSet;//单词过滤

	//将得到的查询单词拆分为单个字母，并依据每个字母找到其索引
	for(size_t i = 0; i != query_.size(); ++i){
		char ch = query_[i];
		string character(charToStr[ch]);
		//string character(1, query_[i]);

		auto iter = indexMap_.find(character);//查找当前字母

		if(iter != indexMap_.end() ){//如果找到
			auto retSet = iter->second;
			for(auto it = retSet.begin();it != retSet.end(); ++it){//遍历该字母对应的索引单词

				res_.word_ = dictionary_[*it].first;//单词

				if(strSet.end() == strSet.find(res_.word_)){//如果该单词是首次出现,对对其进行处理
					strSet.insert(res_.word_);
					res_.count_ = dictionary_[*it].second;//词频
					res_.distance_ = editDistance(res_.word_);//当前单词与查询单词的最小编辑距离
					if(res_.distance_ <= 3){
						resQue_.push(res_);//把当前结果压入优先队列	
					}
				}else{
					continue;
				}

			}
		}
	
	}//end of for
	
	if(!resQue_.empty()){//如果对端输入一串乱码,那么优先队列肯定为空
		conn_->sendInLoop(resQue_.top().word_);//把优先级队列里的第一个元素发给对端
	}else{
		conn_->sendInLoop("sorry,can't find!");
	}
#if 1
	//测试
	int cnt = 5;//打印优先队列中的前五个元素(如果有>=5个元素的话)
	while(!resQue_.empty() && cnt > 0){
		auto res = resQue_.top();
		cout<<res.word_<<"-"<<res.count_<<"-"<<res.distance_<<endl;//打印“单词-词频-编辑距离”
		resQue_.pop();
		--cnt;
	}
#endif
}
