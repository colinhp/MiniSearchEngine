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
		pInstance_ = new Dictionary(dictPath,indexPath);	
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

	ifstream in(dictPath);
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

	in.close();

	ifstream in2(indexPath);
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
}
