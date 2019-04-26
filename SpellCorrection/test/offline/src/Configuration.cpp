 ///
 /// @file    Configuration.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 22:16:55
 ///
 
#include "Configuration.h"

#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::istringstream;

using namespace kk;

//定义静态变量
map<string,string> Configuration::confMap_;//默认初始化
Configuration* Configuration::pInstance_ = NULL;

Configuration* Configuration::getInstance(const string& confPath)
{
	if(pInstance_ == NULL){
		pInstance_ = new Configuration(confPath);
		atexit(destroy);
	}
	return pInstance_;
}

void Configuration::destroy()
{
	if(pInstance_)
		delete pInstance_;
}

Configuration::Configuration(const string& confPath)
{
	ifstream in(confPath);
	if(!in){
		cout<<"Configuration::init() error"<<endl;
		return;
	}
	//配置文件打开成功,则读取信息并存放在map中
	string line;
	string key,value;
	while(getline(in,line)){
		istringstream iss(line);
		iss >> key >> value;
		//cout<<"key = "<<key<<" value = "<<value<<endl;//for debug
		confMap_.insert(make_pair(key,value));	
	}
}

//返回读取的配置文件信息
map<string,string>& Configuration::getConfMap()
{
	return confMap_;
}

