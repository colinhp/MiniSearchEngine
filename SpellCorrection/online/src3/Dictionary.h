 ///
 /// @file    Dictionary.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-27 19:17:58
 ///
 
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__ 

#include <stdlib.h>

#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::istringstream;

namespace kk
{
//采用单例模式读取词典
class Dictionary
{
public:
	static Dictionary* getInstance(const string& enDictPath, const string& chDictPath, const string& enIndexPath, const string& chIndexPath);	
	static void destroy();
	static vector<pair<string,int>>& getDict();
	static map<string,set<int>>& getIndexMap();
private:
	//构造函数私有化
	Dictionary(const string& enDictPath, const string& chDictPath, const string& enIndexPath, const string& chIndexPath);

private:
	static Dictionary* pInstance_;
	static vector<pair<string,int>> dictionary_; 
	static map<string,set<int>> indexMap_;//存放索引
};
}
#endif
