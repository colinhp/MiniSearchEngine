 ///
 /// @file    Dictionary.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-28 10:37:23
 ///
 
#include "Dictionary.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace kk;

//定义静态变量
Dictionary* Dictionary::pInstance_ = NULL;
vector<pair<string,int>> Dictionary::dictionary_;//默认初始化 
map<string,set<int>> Dictionary::indexMap_;//默认初始化

Dictionary* Dictionary::getInstance(const string& dictPath, const string& indexPath)
{
	if(pInstance_ == NULL){
		pInstance_ = new Dictionary(dictPath, indexPath);	
		atexit(destroy);//注册回收函数,在main函数退出时自动回收
	}
	return pInstance_;
}

void Dictionary::destroy()
{
	if(pInstance_)
		delete pInstance_;
}
vector<pair<string,int>>& Dictionary::getDict()
{ return dictionary_; }
	
map<string,set<int>>& Dictionary::getIndexMap()
{ return indexMap_; }


//初始化,读取词典存放至vector<pair<string,int>> dictionary_ 
//       读取索引存放至map<string,set<int>> indexMap_
Dictionary::Dictionary(const string& dictPath, const string& indexPath)
{

	ifstream in1(dictPath);
	if(!in1){
		cout<<"Task open dict.dat error!"<<endl;
		return;
	}

	string line;
	string word,count;
	while(getline(in1,line)){
		istringstream iss(line);
		iss >> word >> count;
		dictionary_.push_back(make_pair(word,stoi(count)));
	}


	ifstream in2(indexPath);
	if(!in2){
		cout<<"Task open dict.index error!"<<endl;
		return;
	}
	string character,idx;
	while(getline(in2,line)){
		istringstream iss(line);
		iss >> character;
		while(iss >> idx){
			indexMap_[character].insert(stoi(idx));
		}
	}

	in1.close();
	in2.close();
}

void Dictionary::show()
{
#if 0
	for(auto iter = dictionary_.begin(); iter != dictionary_.end(); ++iter){
		cout<<iter->first<<" "<<iter->second<<endl;
	}
#endif
	for(size_t i = 0; i != dictionary_.size(); ++i){
		cout<<dictionary_[i].first<<" "<<dictionary_[i].second<<endl;	
	}
	for(auto iter = indexMap_.begin(); iter != indexMap_.end(); ++iter){
		cout<<iter->first<<" ";
		for(auto idx : iter->second){
			cout<<idx<<" ";
		}
		cout<<endl;
	}
}


