 ///
 /// @file    test.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-29 10:07:42
 ///
#include "./cppjieba/Jieba.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::map;
using std::unordered_set;

const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

int main()
{
	cppjieba::Jieba jieba(DICT_PATH,
						  HMM_PATH,
						  USER_DICT_PATH,
						  IDF_PATH,
						  STOP_WORD_PATH);
	vector<string> result;//存放分词结果
	string line;

	ifstream in("yuliao.txt");//打开语料库源文件
	if(!in){
		cout<<"open file error"<<endl;
		return -1;
	}
	
	ifstream in2("stop_words_zh.txt");//打开停用语料库
	if(!in2){
		cout<<"open file error"<<endl;
		return -1;
	}

	unordered_set<string> stop_words;//存放停用词
	while(getline(in2,line)){
//		cout<<line<<endl;
		stop_words.insert(line);
	}
	
	for(auto iter = stop_words.end(); iter != stop_words.end(); ++iter){
		cout<<"----"<<endl;
		cout<<*iter<<endl;
	}

	map<string,int> dict;
	while(getline(in,line)){
//		cout<<line<<endl;
		jieba.Cut(line,result,true);
		for(auto iter = result.begin(); iter != result.end(); ++iter){
			if(stop_words.count(*iter) ==  0){//如果当前单词不属于停用词
				++dict[*iter];
			}
		}
	}
	
	//将词典信息写入文件
	ofstream out("ch_dict.dat");
	if(!out){
		cout<<"open ch_dicti.dat error"<<endl;
		return -1;
	}

	for(auto iter = dict.begin(); iter != dict.end(); ++iter){
		out<<iter->first<<" "<<iter->second<<endl;
	}

	return 0;
}
