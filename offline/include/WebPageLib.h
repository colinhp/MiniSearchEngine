 ///
 /// @file    WebPageLib.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 23:33:03
 ///
 
#ifndef __WEBPAGELIB_H__
#define __WEBPAGELIB_H__

#include "Configuration.h"
#include "DirScanner.h"
#include "RemoveDuplicatePage.h"

#include <vector>

namespace kk
{
struct PageInfo
{
	int docID;//文章的docid
	int offset;//文章在网页库中的起始位置
	int pageLen;//文章的长度
};

class WebPageLib
{
public:
	WebPageLib(const Configuration& conf, const DirScanner& dirScanner, const RemoveDuplicatePage& pageRemover);

	void createWebPageLib();
	void storeWebPageLib();
private:
	Configuration conf_;
	DirScanner dirScanner_;
	vector<string> pages_;//一个string存放一篇文章
	vector<PageInfo> pageInfo_;
	std::vector<uint64_t> pageSimhash_;//存储被存入pages_中的文章的simhash值
	RemoveDuplicatePage pageRemover_;
};

}//end of namespace kk
#endif
