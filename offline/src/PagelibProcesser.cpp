 ///
 /// @file    PagelibProcesser.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-03 16:31:42
 ///
 
#include "../include/Configuration.h"
#include "../include/PagelibProcesser.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

using namespace kk;

PagelibProcesser::PagelibProcesser(Configuration& conf)
:conf_(conf)
{ }

void PagelibProcesser::doProcess()
{
	cout << "----create invert_index_table start----" << endl;
	readFile();
	createInvertIndexTable();
	store();
	cout << "----create invert_index_table done----" << endl;
}

//将磁盘文件(webpage.lib和offset.lib)读入内存
void PagelibProcesser::readFile()
{
	auto & confMap = conf_.getConfMap();
	string pagelib_path = confMap["webpage"];
	string offset_path = confMap["offset"];
	
	ifstream inPagelib(pagelib_path);
	ifstream inOffset(offset_path);
	if(!inPagelib || !inOffset){
		cout << "open webpage.lib or offset.lib error!" << endl;
		return;
	}

	string line;
	int docID, docOffset, docLen;
	while(getline(inOffset,line)){

		istringstream iss(line);	
		iss >> docID >> docOffset >> docLen;
		cout<<"docID: "<<docID<<" docOffset: "<<docOffset<<" docLen: "<<docLen<<endl;

		string doc;//存放一篇网页
		doc.resize(docLen,' ');//初始化docLen长度的空字符串
		inPagelib.seekg(docOffset, inPagelib.beg);//把当前的流位置定位到离流开始位置offset处
		inPagelib.read(&(*doc.begin()), docLen);//从流中读取docLen个字符,写入字符串doc
		
		WebPage page(doc,jieba_);//在WebPage内部,对文档做了处理,通过WebPage的对象可以拿到文档的docid，url等
		pagelib_.push_back(page);
		offsetlib_.insert({docID,{docOffset,docLen}});
	}

	inPagelib.close();
	inOffset.close();
}

void PagelibProcesser::createInvertIndexTable()
{
	for(auto iter = pagelib_.begin(); iter != pagelib_.end(); ++iter){
		auto docWordMap = (*iter).getDocWordMap();
		for(auto it = docWordMap.begin(); it != docWordMap.end(); ++it){
			//倒排索引的结构unordered_map<string, vector<pair<int, double>>> invertIndexTable_
			invertIndexTable_[it->first].push_back({iter->getDocID(),it->second});//<单词,<docID,词频>>
		}
	}

	size_t totalPageNum = pagelib_.size();//网页库中网页的总数
	map<int,double> pageWeight;//每个网页的权重和
	for(auto iter = invertIndexTable_.begin(); iter != invertIndexTable_.end(); ++iter){
		size_t df = iter->second.size();//文档频率,表示单词在多少篇文档中出现过	
		double idf = log10(static_cast<double>(totalPageNum)/(df+1));//逆文档频率
		for(auto & elem : iter->second){
			double weight = elem.second * idf;
			elem.second = weight;
			pageWeight[elem.first] += pow(weight,2);
		}
	}

	//归一化处理
	for(auto iter = invertIndexTable_.begin(); iter != invertIndexTable_.end(); ++iter){
		for(auto & elem : iter->second){
			elem.second = elem.second / sqrt(pageWeight[elem.first]);
		}
	}
}

void PagelibProcesser::store()
{
	auto & confMap = conf_.getConfMap();
	string invertIndexTable_path = confMap["invert_index_table"];
	ofstream out(invertIndexTable_path);
	if(!out){
		cout << "open invert_index_table error!" << endl;
		return;
	}

	for(auto iter = invertIndexTable_.begin(); iter != invertIndexTable_.end(); ++iter){
		out << iter->first << " ";//写入单词		
		auto & retVec = iter->second;
		for(auto it = retVec.begin(); it != retVec.end(); ++it){//写入倒排索引项(docID,在本文档中的词频)
			out << it->first << " " << it->second << " ";
		}
		out << endl;
	}

	out.close();
}

