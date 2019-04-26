 ///
 /// @file    WordSegmentation.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-29 20:26:37
 ///
 
#ifndef __WORDSEGMENTATION_H__
#define __WORDSEGMENTATION_H__

#include "./cppjieba/Jieba.hpp"


#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;

const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

namespace kk
{
class WordSegmentation
{
public:
	WordSegmentation()
	:jieba_(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
	{}

	void cut(const string& line,vector<string>& result)
	{
		jieba_.Cut(line,result,true);
	}
private:
	cppjieba::Jieba jieba_;
};

}//end of namespace kk

#endif
