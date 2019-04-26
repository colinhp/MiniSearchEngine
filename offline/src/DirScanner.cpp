 ///
 /// @file    DirScanner.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 20:48:43
 ///
 
#include "../include/DirScanner.h"

#include <dirent.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace kk;

DirScanner::DirScanner(const Configuration& conf)
: conf_(conf)
{
	assembleFilePath();
}

void DirScanner::assembleFilePath()
{
	auto & confMap = conf_.getConfMap();
	string yuliao_path = confMap["yuliao"];//语料库所在的路径

	DIR* pDir =  opendir(yuliao_path.c_str());
	if(pDir == NULL){
		cout << "open rss dir error!" << endl;
		return;
	}

	struct dirent* pDirInfo;

	string file_path;
	//获取语料库下每个xml文件的文件名(注意会有 .. 和 . 这两个目录名)
	while((pDirInfo = readdir(pDir)) != NULL){
		string filename(pDirInfo->d_name);	
		//cout << filename << endl;
		if(filename != "." && filename != ".."){
			file_path = yuliao_path + filename;
			filePath_.push_back(file_path);
		}
	}
}

vector<string>& DirScanner::getFilePath()
{
	return filePath_;
}

void DirScanner::debug()
{
	cout << "****Rss语料库文件信息****" << endl;
	for(auto iter = filePath_.begin(); iter != filePath_.end(); ++iter){
		cout << *iter << endl;
	}
	cout << "*************************" << endl;
}
