 ///
 /// @file    WebPageLib.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-01 10:31:33
 ///
 
#include "../include/WebPageLib.h"
#include "../include/RssReader.h"
#include "../include/RemoveDuplicatePage.h"

#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ofstream;
using std::ostringstream;

using namespace kk;

WebPageLib::WebPageLib(const Configuration& conf, const DirScanner& dirScanner,const RemoveDuplicatePage& pageRemover)
: conf_(conf)
, dirScanner_(dirScanner)
, pageRemover_(pageRemover)
{ }

void WebPageLib::createWebPageLib()
{
	//1.获取rss语料库中所有文件的路径集合
	vector<string>& xmlFilePath = dirScanner_.getFilePath();

	//2.解析rss语料库中所有文件
	RssReader rssReader(xmlFilePath,pageRemover_);
	rssReader.parseAllRss();
	vector<RssItem>& articles = rssReader.getArticles();

	//3.将文章内容读取至vector<string> pages_
//	struct PageInfo page_info;
//	int offset = 0;

	cout << "****articles.size() = " << articles.size() << "****" << endl;

	for(size_t i = 0; i != articles.size(); ++i)
	{
#if 0
		string docID = std::to_string(i+1);//整型-->string型
		page = "<doc>\n\t<docid>" + docID + "</docid>\n" +
				   "\t<title>" + articles[i].title + "</title>\n" +
				   "\t<link>" + articles[i].link + "</link>\n" +
				   "\t<content>" + articles[i].content + "</content>\n" +
				   "</doc>";
#endif
		ostringstream oss;
		oss << "<doc>\n\t<docid>" << (i+1) << "</docid>\n" 
				   << "\t<title>" << articles[i].title << "</title>\n" 
				   << "\t<link>" << articles[i].link << "</link>\n" 
				   << "\t<content>" << articles[i].content << "</content>\n" 
				   << "</doc>";

		string page = oss.str();

		//遍历pageSimhash_，比较当前要插入的page的simhash值
		//如果在pages_中存在与之类似的（即海明距离小于3）则丢弃
		//仅当海明距离大于3时才push
#if 1
		size_t j;
		for(j = 0; j != pageSimhash_.size(); ++j){
			if(pageRemover_.isSame(articles[i].simhash, pageSimhash_[j])){
				cout << "has found a duplicate page,[docID]=" << (i+1) << endl;
				break;
			}		
		}	
#endif		
		if(j == pageSimhash_.size()){//说明没有相似的文章
			pages_.push_back(page);
			pageSimhash_.push_back(articles[i].simhash);
			
			cout << "non-duplicated page,[docID]=" << (i+1) << endl;
#if 0			
			//建立网络偏移库offset.lib,信息存入pageInfo
			page_info.docID = i+1;
			page_info.offset = offset;
			offset += page.size(); 
			page_info.pageLen = page.size();
			pageInfo_.push_back(page_info);
#endif
		}

	}
}

void WebPageLib::storeWebPageLib()
{
	auto& confMap = conf_.getConfMap();
	string filename = confMap["webpage"];
	ofstream os(filename);
	if(!os)
	{
		cout<<"open file ["<<filename<<"] error"<<endl;
		return;
	}

	string filename2 = confMap["offset"];
	ofstream os2(filename2);
	if(!os2)
	{
		cout<<"open file ["<<filename2<<"] error"<<endl;
		return;
	}

	for(size_t i = 0; i != pages_.size(); ++i){
		int docid = i + 1;
		int doclen = pages_[i].size();
		ofstream::pos_type offset = os.tellp();//网页库流的位置
		os2 << docid <<" " << offset <<" " << doclen << '\n';
		os << pages_[i] << endl;

		cout << "----storing page[" << i+1 <<"]----" << endl;
	}
	cout << "----storing page done----" << endl;
#if 0
	//网络偏移库信息存入offset.lib
	filename = confMap["offset"];
	ofstream os2(filename);
	if(!os2)
	{
		cout<<"open file ["<<filename<<"] error"<<endl;
		return;
	}
	
	for(size_t i = 0; i != pageInfo_.size(); ++i ){
		os2 << pageInfo_[i].docID << " "
			<< pageInfo_[i].offset << " "
			<< pageInfo_[i].pageLen << endl;
	}	
#endif
	os.close();
	os2.close();

}
