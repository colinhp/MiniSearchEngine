 ///
 /// @file    test.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 21:34:43
 ///
 
#include "Configuration.h"
#include "DictProducer.h"

#include <string>
#include <map>
using std::string;
using std::map;

using namespace kk;

int main()
{
	string confPath("/home/kkmjy/C++/SpellCorrection/offline/conf/conf.txt");
	Configuration* pConf = Configuration::getInstance(confPath);
	map<string,string>& confMap = pConf->getConfMap();

	DictProducer dictProducer(confMap);
	dictProducer.createEnglishDictionary();//创建英文词典
	dictProducer.createChineseDictionary();//创建中文词典
	
	return 0;
}
