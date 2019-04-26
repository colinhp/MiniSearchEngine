 ///
 /// @file    Configuration.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 22:16:55
 ///
 
#include "Configuration.h"
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

Configuration::Configuration(const string& confPath)
:path_(confPath)
{
	ifstream in(path_);
	//配置文件打开失败
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

