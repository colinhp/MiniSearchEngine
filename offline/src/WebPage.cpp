 ///
 /// @file    WebPage.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 15:24:34
 ///

#include "../include/WebPage.h"
#include "../include/WordSegmentation.h"

#include <iostream>
using std::cout;
using std::endl;
using namespace kk;

WebPage::WebPage(const string& doc, WordSegmentation& jieba)
:doc_(doc)
{
	process(doc, jieba);
}

void WebPage::process(const string& doc,  WordSegmentation& jieba)
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

	//创建词典
	createWordMap(docContent_,jieba);
}

void WebPage::createWordMap(const string& content, WordSegmentation& jieba)
{
	vector<string> result;
	cout <<"========" << endl;
	jieba.cut(content,result);
	cout <<"@@@@@@@@" << endl;
	for(auto iter = result.begin(); iter != result.end(); ++iter){
		++docWordMap_[*iter];
	}
}

string WebPage::getDoc()
{ return doc_; }

int WebPage::getDocID()
{ return docID_; }

string WebPage::getDocTitle()
{ return docTitle_; }

string WebPage::getDocUrl()
{ return docUrl_; }

map<string,int> WebPage::getDocWordMap()
{ return docWordMap_; }

