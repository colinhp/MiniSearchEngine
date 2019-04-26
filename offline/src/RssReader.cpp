 ///
 /// @file    RssReader.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 23:08:38
 ///

#include "../include/RssReader.h"
#include "../include/tinyxml2.h"
#include "../include/RemoveDuplicatePage.h"

#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

using namespace kk;
using namespace tinyxml2;

RssReader::RssReader(vector<string>& xmlFilePath,RemoveDuplicatePage& pageRemover)
: xmlFilePath_(xmlFilePath)
, pageRemover_(pageRemover)
{}

//解析语料库中的所有rss文件
void RssReader::parseAllRss()
{
	for(size_t i = 0; i != xmlFilePath_.size(); ++i){
		parseOneRss(xmlFilePath_[i]);
		cout << "----loading rss file " << i+1 << "----" << endl;
	}	
	cout << "----loading rss file done----" << endl;
}

//解析一篇rss文件
void RssReader::parseOneRss(const string & filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());//打开欲处理文件
	if(doc.ErrorID() != 0 )
	{
		cout<<"open file ["<<filename<<"] error"<<endl;
		return;
	}

	XMLElement* rootElement = doc.FirstChildElement("rss");
	XMLElement* itemElement = rootElement->FirstChildElement("channel")->FirstChildElement("item");

	do{
		XMLElement* elementTitle = itemElement->FirstChildElement("title");
		XMLElement* elementLink = itemElement->FirstChildElement("link");
		XMLElement* elementDescription = itemElement->FirstChildElement("description");
		XMLElement* elementContent = itemElement->FirstChildElement("content:encoded");
	
		string title = elementTitle->GetText();
		string link = elementLink->GetText();
		string description = elementDescription->GetText();
		string content = elementContent->GetText();
	
		//用正则表达式处理
		boost::regex r("<.+?>");
		string processContent = boost::regex_replace(content,r,"");
		
		RssItem item;
		item.title = title;
		item.link = link;
		item.description = description;
		item.content = processContent;
		//扩展：获取文章的simhash值
		item.simhash = pageRemover_.getSimhashValue(processContent);
	
		articles_.push_back(item);

	}while((itemElement = itemElement->NextSiblingElement()));//下一个兄弟节点
}

vector<RssItem>& RssReader::getArticles()
{
	return articles_;
}

