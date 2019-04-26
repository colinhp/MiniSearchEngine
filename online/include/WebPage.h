 ///
 /// @file    WebPage.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 14:53:18
 ///
 
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "WordSegmentation.h"

#include <vector>
#include <string>
#include <map>
#include <set>
using std::string;
using std::map;
using std::set;
using std::vector;

namespace kk
{
class WebPage
{
public:
	WebPage(const string& doc, Configuration& conf); 
	WebPage(){}
	string getDoc();
	int getDocID();
	string getDocTitle();
	string getDocUrl();
	string getDocSummary(const vector<string>& querywords); 

private:
	void process(const string& doc);

private:
	string doc_;//存储整篇文档
	Configuration conf_;
	int docID_;
	string docTitle_;
	string docUrl_;
	string docContent_;
	string docSummary_;//存放摘要
};

}//end of namespace kk
#endif
