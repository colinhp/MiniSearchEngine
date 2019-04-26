 ///
 /// @file    RemoveDuplicatePage.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-02 12:58:57
 ///
 

#include "../include/RemoveDuplicatePage.h"

using namespace simhash;
using namespace kk;

RemoveDuplicatePage* RemoveDuplicatePage::pInstance_ = NULL;
Simhasher RemoveDuplicatePage::simhasher_(DICTPATH, HMMPATH, IDFPATH, STOPWORDPATH);
uint64_t RemoveDuplicatePage::u64_ = 0;
vector<pair<string ,double> > RemoveDuplicatePage::res_;

RemoveDuplicatePage* RemoveDuplicatePage::getInstance()
{
	if(pInstance_ == NULL){
		pInstance_ = new RemoveDuplicatePage();
		atexit(destroy);
	}
	return pInstance_;
}

void RemoveDuplicatePage::destroy()
{
	if(pInstance_)
		delete pInstance_;
}

//根据传入的文本内容求得simhash值
uint64_t RemoveDuplicatePage::getSimhashValue(const string & text)
{
	simhasher_.extract(text, res_, topN);//提取文本text的关键词
	simhasher_.make(text, topN, u64_);//求该文本对应的simhash值
	return u64_;
}

//若两篇文章的海明距离小于3，则认为这两篇文章相同，返回true
bool RemoveDuplicatePage::isSame(uint64_t lhs, uint64_t rhs)
{
	//simhasher_.isEqual(lhs,rhs,5);//最后一个为默认参数,默认为3
	return 	simhasher_.isEqual(lhs,rhs);	
}	

//打印关键词序列，res_ 内部重载了<<，故可以直接cout
void RemoveDuplicatePage::debug()
{
	cout << "关键词序列是: " << res_ << endl;
}

