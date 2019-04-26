 ///
 /// @file    test.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-02 10:35:59
 ///

#include "simhash/Simhasher.hpp"
#include "WordSegmentation.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;

using namespace simhash;

int main()
{
	Simhasher simhasher("./dict/jieba.dict.utf8", "./dict/hmm_model.utf8", "./dict/idf.utf8", "./dict/stop_words.utf8");
	string s("我想成为一名优秀的程序员。");	
	kk::WordSegmentation jieba;
	vector<string> wordVec;
	jieba.cut(s,wordVec);
	cout << "wordVec.size = " << wordVec.size() << endl;
	for(auto & elem : wordVec)
	{
		cout << "[" << elem << "]" << " ";
	}
	cout << endl;

	size_t topN = 5;
	uint64_t u64 = 0;
	vector<pair<string ,double> > res;//存放关键字及其权重
	simhasher.extract(s, res, topN);//从文档s中提取topN个关键词存放至res中
	simhasher.make(s, topN, u64);
	cout<< "文本：\"" << s << "\"" << endl;
	cout << "关键词序列是: " << res << endl;
	cout<< "simhash值是: " << u64 << endl;

	uint64_t u1 = 100010110110;
	uint64_t u2 = 110001110011;
	cout << "u1 与 u2是否相等: " << simhasher.isEqual(u1,u2) << endl;//海明距离是5,大于3应该是不相等
	return 0;
}

