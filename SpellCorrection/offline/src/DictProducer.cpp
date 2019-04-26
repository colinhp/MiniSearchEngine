 ///
 /// @file    DictProducer.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 21:39:58
 ///
 
#include "CutByCharacter.h"
#include "DictProducer.h"

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <regex>
using std::cout;
using std::endl;
using std::regex;
using std::smatch;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::pair;
using std::set;

using namespace kk;

DictProducer::DictProducer(const map<string,string>& confMap) 
: confMap_(confMap)
{}


//读取源文件,创建词典
void DictProducer::createDictionary()
{
	string pathEn = confMap_["english_origin"];
	ifstream in1(pathEn);
	if(!in1){
		cout<<"DictProducer::createEnglishDictionary() open origin file error!"<<endl;
		return;
	}

	readStopWord("en_stop_word");

	string line;
	string word;
	while(getline(in1,line)){
		istringstream iss(line);
		while(iss >> word){
			word = regexMatch(word);//利用正则剔除无关字符	
			word = toLowerCase(word);//大写-->小写
			if(!isStopWord(word)){//如果不是停用词才加入词典
				++dict_[word];
			}
		}
	}

	string pathCh = confMap_["chinese_origin"];
	ifstream in2(pathCh);
	if(!in2){
		cout<<"DictProducer::createChineseDictionary() open origin file error!"<<endl;
		return;
	}

	vector<string> result;//存放分词结果
	WordSegmentation wordSeg;	

	readStopWord("ch_stop_word");//先读取中文停用词的语料库

	while(getline(in2,line)){
		wordSeg.cut(line,result);//对一行文字进行分词,并把分词结果存入result
		for(auto iter = result.begin(); iter != result.end(); ++iter){
			//对存入的单词需进一步精细化处理,如剔除停用词等
			if(!isStopWord(*iter)){
				++dict_[*iter];
			}
		}
	}

	storeDict("dict");

	in1.close();	
	in2.close();
}

void DictProducer::createIndexTable()
{
	vector<pair<string,int>> vDict;
	for(auto iter = dict_.begin(); iter != dict_.end(); ++iter){
		vDict.push_back({iter->first,iter->second});	
	}

	map<string,set<int>> indexTable;
	//遍历每个单词
	for(size_t i = 0; i != vDict.size(); ++i){
		string& word = vDict[i].first;//取出单词
		//遍历每个单词中的字符
		for(size_t j = 0; j != word.size();  ){
			int nbyte = nBytesCode(word[j]);
			//截取单个字符(英文--单个字母;中文--单个汉字)
			string ch = word.substr(j,nbyte);
			j += nbyte;
			auto& retSet = indexTable[ch];
			retSet.insert(i);
		}	
	}

	string path_index = confMap_["index"];
	ofstream out(path_index);
	if(!out){
		cout<<"create IndexTable error"<<endl;
		return;
	}

	//写入索引文件
	for(auto iter = indexTable.begin(); iter != indexTable.end(); ++iter){
		out << iter->first << " ";
		auto& retSet = iter->second;
		for(auto iter = retSet.begin(); iter != retSet.end(); ++iter){
			out << *iter << " ";
		}
		out << endl;
	}
}

void DictProducer::storeDict(const string& path)
{
	//把创建好的字典写入文件中
	string path_dat = confMap_[path];
	ofstream out(path_dat);
	if(!out){
		cout<<"DictProducer::storeDict error!"<<endl;
	}
	for(auto iter = dict_.begin(); iter != dict_.end(); ++iter){
		out << iter->first <<" "<<iter->second<<endl;
	}
	out.close();
}

void DictProducer::readStopWord(const string & path)
{
	string path_stop_word = confMap_[path];
	ifstream in(path_stop_word);
	if(!in){
		cout << "open " << path  << " error!" << endl;
		return;
	}
	string stop_word;
	hashTable_.clear();//先清除，以免中英文混合

	while(getline(in,stop_word)){
		hashTable_.insert(stop_word);	
	}
}

bool DictProducer::isStopWord(const string & word)
{
	if(hashTable_.find(word) != hashTable_.end())
		return true;
	else
		return false;
}

//大写转换成小写
string& DictProducer::toLowerCase(string & word)
{
	for(size_t i = 0;i < word.size();++i)
	{
		if(word[i]>='A' && word[i]<='Z')
			word[i] -= 'A'-'a';
	}
	return word;	
}

//利用正则表达式进行匹配，取出形如"1.hello","Shenzhen."这类带标点的词
string& DictProducer::regexMatch(string & str)
{
	string pattern("[[:alpha:]]+");
	regex r(pattern);
	smatch result;
	if(regex_search(str,result,r))
		str =  result.str();
	return str;
}
