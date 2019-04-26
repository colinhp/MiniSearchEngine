 ///
 /// @file    RssReader.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 23:00:37
 ///
 
#ifndef __RSSREADER_H__
#define __RSSREADER_H__ 

#include "RemoveDuplicatePage.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace kk
{
struct RssItem
{
	string title;
	string link;
	string description;
	string content;
	uint64_t simhash;//存放每篇文章对应的simhash值
};

class RssReader
{
public:
	RssReader(vector<string>& xmlFilePath,RemoveDuplicatePage& pageRemover);

	void parseAllRss();
	void parseOneRss(const string & filename);//解析rss文件
	vector<RssItem>& getArticles();
private:
	vector<RssItem> articles_;
	vector<string> xmlFilePath_;//语料库中所有xml文件的路径集合
	RemoveDuplicatePage pageRemover_;
};

}//end of namespace kk;
#endif
