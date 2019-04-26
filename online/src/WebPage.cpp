 ///
 /// @file    WebPage.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 15:24:34
 ///

#include "../include/WebPage.h"
#include "../include/WordSegmentation.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <utility>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::pair;
using std::string;

using namespace kk;

WebPage::WebPage(const string& doc, Configuration& conf)
: doc_(doc)
, conf_(conf)
{
	process(doc_);
}

void WebPage::process(const string& doc)
{
	string idHead = "<docid>";
	string idTail = "</docid>";
	string urlHead = "<link>";
	string urlTail = "</link>";
	string titleHead = "<title>";
	string titleTail = "</title>";
	string contentHead = "<content>";
	string contentTail = "</content>";

	//提取docid
	int begpos = doc.find(idHead);	
	int endpos = doc.find(idTail);
	string docID = doc.substr(begpos + idHead.size(), endpos - begpos - idHead.size());
	docID_ = stoi(docID);

	//提取url
	begpos = doc.find(urlHead);	
	endpos = doc.find(urlTail);
	docUrl_ = doc.substr(begpos + urlHead.size(), endpos - begpos - urlHead.size());

	//提取title
	begpos = doc.find(titleHead);	
	endpos = doc.find(titleTail);
	docTitle_ = doc.substr(begpos + titleHead.size(), endpos - begpos - titleHead.size());
		
	//提取content
	begpos = doc.find(contentHead);	
	endpos = doc.find(contentTail);
	docContent_ = doc.substr(begpos + contentHead.size(), endpos - begpos - contentHead.size());
}

string WebPage::getDoc()
{ return doc_; }

int WebPage::getDocID()
{ return docID_; }

string WebPage::getDocTitle()
{ return docTitle_; }

string WebPage::getDocUrl()
{ return docUrl_; }

string WebPage::getDocSummary(const vector<string>& queryWords)
{
	vector<string> summaryVec;
	istringstream iss(docContent_);
	string line;
	while(iss >> line){
		for(auto iter = queryWords.begin(); iter != queryWords.end(); ++iter){
			if(line.find(*iter) != string::npos){
				summaryVec.push_back(line);
				break;//跳出for
			}
		}
		if(summaryVec.size() >= 5){
			break;
		}
	}

	for(auto iter = summaryVec.begin();iter != summaryVec.end();++iter){
		docSummary_ += *iter;
	}
	return docSummary_;
}
