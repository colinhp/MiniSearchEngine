 ///
 /// @file    WebPage.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 14:53:18
 ///
 
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "WordSegmentation.h"

#include <string>
#include <map>
using std::string;
using std::map;

namespace kk
{
class WebPage
{
public:
	WebPage(const string& doc, WordSegmentation& jieba);

	string getDoc();
	int getDocID();
	string getDocTitle();
	string getDocUrl();
	string getDocSummary();
	map<string,int> getDocWordMap();

private:
	void process(const string& doc, WordSegmentation& jieba);
	void createWordMap(const string& content, WordSegmentation& jieba);//生成词典
private:
	string doc_;//存储整篇文档
	int docID_;
	string docTitle_;
	string docUrl_;
	string docContent_;
	string docSummary_;//???
	map<string,int> docWordMap_;//存储本文档的单词词典
};

}//end of namespace kk
#endif
