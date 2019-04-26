 ///
 /// @file    Cache.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-27 09:36:40
 ///
 
#ifndef __CACHE_H__
#define __CACHE_H__

#include <iostream>
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;

namespace kk
{
class Cache
{
public:
	Cache(){}
	Cache(const Cache& );

	void addElement(const string& key, const string& value);//往缓存中添加数据
	void readFromFile(const string& filename);//从文件中读取缓存此信息
	void writeToFile(const string& filename);//将缓存信息写入文件
	void update(const Cache& rhs);//更新缓存信息

	bool hasQueryResult(const string& query);//判断要查询的单词是否已经存在
	string& getResult(const string& query);//获取查询结果

	//测试用,显示cache的内容
	void show();
private:
	unordered_map<string,string> hashMap_;//存放查询单词与对应结果
};

}//end of namespace kk

#endif
