 ///
 /// @file    Configuration.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 19:59:00
 ///
 
#include "../include/Configuration.h"

#include <fstream>
#include <sstream>

#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

using namespace kk;

//静态数据成员的初始化
Configuration* Configuration::pInstance_ = NULL;
map<string,string> Configuration::confMap_;

Configuration::Configuration(const string& conf_path)
{
	ifstream in(conf_path);
	if(!in){
		cout << "open conf file error!" << endl;
		return;
	}
	
	string line;
	string key,path;

	//读取配置文件信息
	while(getline(in,line)){
		istringstream iss(line);
		iss >> key >> path;
		confMap_.insert({key, path});
	}
}

Configuration* Configuration::getInstance(const string& conf_path)
{
	if(pInstance_ == NULL){
		pInstance_ = new Configuration(conf_path);
		atexit(destroy);//注册自动销毁函数
	}
	return pInstance_;
}

map<string,string>& Configuration::getConfMap()
{
	return confMap_;
}

void Configuration::debug()
{
	cout << "****配置文件信息****" << endl;
	for(auto iter : confMap_){
		cout << iter.first << " " << iter.second << endl;
	}
	cout << "********************" << endl;
}

void Configuration::destroy()
{
	if(pInstance_)
		delete pInstance_;
}
