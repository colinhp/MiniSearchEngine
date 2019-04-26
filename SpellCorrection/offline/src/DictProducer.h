 ///
 /// @file    DictProducer.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 21:31:23
 ///
#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__ 

#include "Configuration.h"
#include "WordSegmentation.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
using std::string;
using std::vector;
using std::map;
using std::unordered_set;

namespace kk
{
class DictProducer
{
public:
	DictProducer(const map<string,string> & confMap);

//	void createEnglishDictionary();
//	void createChineseDictionary();

	void createDictionary();
	void createIndexTable();//创建索引文件,并写入filename中
	void storeDict(const string & filename);//将词典内容写入文件中

private:
	void readStopWord(const string & path);//读取停用词的语料库
	bool isStopWord(const string & word);//判断单词是否为停用词,若是,则不需要加入词典

	string& toLowerCase(string& word);//大写转换成小写,仅供类成员函数使用
	//利用正则表达式进行匹配，取出形如"1.hello","Shenzhen."这类带标点的词
	string& regexMatch(string & str);

private:
	map<string,int> dict_;//生成的词典
	map<string,string> confMap_;//配置文件的信息
	unordered_set<string> hashTable_;
};

}//end of namespace kk
#endif
