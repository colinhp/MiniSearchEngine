 ///
 /// @file    Cache.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-27 09:47:50
 ///
 
#include "Cache.h"
#include <fstream>
#include <sstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;

using namespace kk;

//拷贝构造函数
Cache::Cache(const Cache& rhs)
{
	for(auto iter = rhs.hashMap_.begin(); iter != rhs.hashMap_.end(); ++iter){
		hashMap_.insert({iter->first,iter->second});
	}
}

//向内存Cache写入信息
void Cache::addElement(const string& key,const string& value)
{
	hashMap_.insert({key,value});
	cout<<"current result has written into cache"<<endl;
}

//把磁盘Cache的内容读取到内存Cache
void Cache::readFromFile(const string& filename)
{
	ifstream in(filename);
	if(!in){
		cout<<"Cache::readFromFile,open Cache file error!"<<endl;
		return;
	}

	string line;
	string key,value;
	while(getline(in,line)){
		istringstream iss(line);
		iss >> key >> value;
		hashMap_.insert({key,value});
	}
}

//把内存Cache的内容写入磁盘Cache
void Cache::writeToFile(const string& filename)
{
//	ofstream out(filename,ofstream::app);//每次写入文件，都定位到文件尾
	ofstream out(filename);
	if(!out){
		cout<<"Cache::writeToFile,open Cache file error!"<<endl;
		return;
	}
	
	for(auto iter = hashMap_.begin(); iter != hashMap_.end(); ++iter){
		out << iter->first << " " << iter->second << endl;
	}
}

//更新当前内存Cache,把rhs的值插入
void Cache::update(const Cache& rhs)
{
	for(auto iter = rhs.hashMap_.begin(); iter != rhs.hashMap_.end(); ++iter){
		hashMap_.insert({iter->first,iter->second});
	}
	cout<<"cache updated"<<endl;
}

bool Cache::hasQueryResult(const string& query)
{
	if(hashMap_.end() == hashMap_.find(query))
		return false;
	else
		return true;
}

string& Cache::getResult(const string& query)
{
	return hashMap_[query];
}

void Cache::show()
{
	for(auto iter = hashMap_.begin(); iter != hashMap_.end(); ++iter){
		cout << iter->first <<" "<<iter->second<<endl;
	}
}
