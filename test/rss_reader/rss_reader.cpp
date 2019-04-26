 ///
 /// @file    rss_reader.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-07 12:12:01
 ///

#include "tinyxml2.h"

#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;
using namespace tinyxml2;

struct RssItem
{
	string title;
	string link;
	string description;
	string content;
};

class RssReader
{
public:
	RssReader(){}

	void parseRss(const string & filename);
	void dump(const string & filename);
	void debug();
private:
	vector<RssItem> articles_;
};

void RssReader::parseRss(const string & filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());//打开欲处理文件
	if(doc.ErrorID() != 0 )
	{
		cout<<"open file ["<<filename<<"] error"<<endl;
		return;
	}

//	doc.Parse(filename.c_str());

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
	
		articles_.push_back(item);
	}while((itemElement = itemElement->NextSiblingElement()));//下一个兄弟节点
}

void RssReader::dump(const string & filename)
{
	ofstream os(filename);
	if(!os)
	{
		cout<<"open file ["<<filename<<"] error"<<endl;
		return;
	}

	for(size_t i=0; i!=articles_.size(); ++i)
	{
		os<<"<doc>"<<endl
		  <<"\t<docid>"<<i+1<<"</docid>"<<endl
		  <<"\t<title>"<<articles_[i].title<<"</title>"<<endl
		  <<"\t<link>"<<articles_[i].link<<"</link>"<<endl
		  <<"\t<content>"<<articles_[i].content<<"</content>"<<endl
		  <<"</doc>"<<endl<<endl;;
	}

	os.close();
}


void RssReader::debug()
{
	string page;
	for(size_t i=0; i!=articles_.size(); ++i)
	{
		string docID = std::to_string(i+1);//整型-->string型
		page = "<doc>\n\t<docid>" + docID + "</docid>\n" +
				   "\t<title>" + articles_[i].title + "</title>\n" +
				   "\t<link>" + articles_[i].link + "</link>\n" +
				   "\t<content>" + articles_[i].content + "</content>\n" +
				   "</doc>";
		cout <<"----page[" << i+1 << "]----"<< endl;
		cout << page << endl;	
		cout <<"---------------------------"<< endl; 
	}

}

int main()
{
	RssReader rss_reader;
	rss_reader.parseRss("/home/kkmjy/C++/RssMiniSearchEngine/rss_reader/coolshell.xml");
//	rss_reader.dump("pagelib.dat");
	rss_reader.debug();
	return 0;
}
