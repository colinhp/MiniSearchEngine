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


//读取源文件,创建英文词典以及索引
void DictProducer::createEnglishDictionary()
{
	string pathEn = confMap_["english_origin"];
	ifstream in(pathEn);
	if(!in){
		cout<<"DictProducer::createEnglishDictionary() open origin file error!"<<endl;
		return;
	}
	
	dict_.clear();//先清除原有的元素,不然中英文词典就写在一起了

	readStopWord("en_stop_word");

	string line;
	string word;
	while(getline(in,line)){
		istringstream iss(line);
		while(iss >> word){
			word = regexMatch(word);//利用正则剔除无关字符	
			word = toLowerCase(word);//大写-->小写
			if(!isStopWord(word)){//如果不是停用词才加入词典
				++dict_[word];
			}
		}
	}

	createIndexTable(dict_, "en_index");//创建索引文件,存入的地址由配置文件根据en_index去寻找
	storeDict("en_dict");//写入文件

	in.close();
}

//读取源文件,创建中文词典
void DictProducer::createChineseDictionary()
{
	string pathCh = confMap_["chinese_origin"];
	ifstream in(pathCh);
	if(!in){
		cout<<"DictProducer::createChineseDictionary() open origin file error!"<<endl;
		return;
	}

	dict_.clear();//先清除原有的元素,不然中英文词典就写在一起了

	string line,word;
	vector<string> result;//存放分词结果
	WordSegmentation wordSeg;	

	readStopWord("ch_stop_word");//先读取中文停用词的语料库

	while(getline(in,line)){
		wordSeg.cut(line,result);//对一行文字进行分词,并把分词结果存入result
		for(auto iter = result.begin(); iter != result.end(); ++iter){
			//对存入的单词需进一步精细化处理,如剔除停用词等
			if(!isStopWord(*iter)){
				++dict_[*iter];
			}
		}
	}

	createIndexTable(dict_, "ch_index");//创建索引文件,存入的地址由配置文件根据ch_index去寻找
	storeDict("ch_dict");

	in.close();	
}

void DictProducer::createIndexTable(map<string,int>& dict, const string & filename)
{
	vector<pair<string,int>> vDict;
	for(auto iter = dict.begin(); iter != dict.end(); ++iter){
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

	string path_index = confMap_[filename];
	ofstream out(path_index);
	if(!out){
		cout<<"create "<<filename<<" IndexTable error"<<endl;
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
