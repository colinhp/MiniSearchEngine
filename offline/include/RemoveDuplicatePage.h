 ///
 /// @file    RemoveDuplicatePage.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-02 12:58:57
 ///
 
#ifndef __REMOVEDUPLICATEPAGE_H__
#define __REMOVEDUPLICATEPAGE_H__ 

#include "simhash/Simhasher.hpp"
#include "Configuration.h"
#include <stdlib.h>

#include <string>
#include <vector>
#include <utility>
#include <iostream>
using std::string;
using std::vector;
using std::pair;
using std::cout;
using std::endl;

using namespace simhash;

namespace kk
{

const char* const DICTPATH = "./include/dict/jieba.dict.utf8";
const char* const HMMPATH = "./include/dict/hmm_model.utf8";
const char* const IDFPATH = "./include/dict/idf.utf8";
const char* const STOPWORDPATH = "./include/dict/stop_words.utf8";


const size_t topN = 5;//每篇文章提取5个关键字

class RemoveDuplicatePage
{
public:
	static RemoveDuplicatePage* getInstance();
	
	static void destroy();

	//根据传入的文本内容求得simhash值
	static uint64_t getSimhashValue(const string & text);

	//若两篇文章的海明距离小于3，则认为这两篇文章相同，返回true
	static bool isSame(uint64_t lhs, uint64_t rhs);

	//打印关键词序列
	static void debug();

private:
	RemoveDuplicatePage()
	{}
private:
	static RemoveDuplicatePage* pInstance_;
	static Simhasher simhasher_;
	static uint64_t u64_;
	static vector<pair<string ,double> > res_;//存放关键词及其权重
};

}//end of namespace kk

#endif
