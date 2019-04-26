 ///
 /// @file    WordQuery.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-07 12:34:30
 ///
 
#include "../include/WordQuery.h"

#include <math.h>
#include <json/json.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

using namespace kk;

//自定义比较类,比较显示给用户的网页顺序
class Compare
{
public:
	Compare(vector<double> & base)
	: base_(base)
	{}

	bool operator()(const pair<int, vector<double>> & lhs,const pair<int, vector<double>> & rhs)
	{
		double lhsCrossProduct = 0;
		double rhsCrossProduct = 0;
		double lhsVectorLength = 0;
		double rhsVectorLength = 0;
		double baseVectorLength = 0;

		for(size_t index = 0; index != base_.size(); ++index)
		{
																  //lhs*base = x1*b1+x2*b2+...+xn*bn
			lhsCrossProduct += (lhs.second)[index] * base_[index];//分子，即(x1*b1+x2*b2+...+xn*bn)
			rhsCrossProduct += (rhs.second)[index] * base_[index];
			lhsVectorLength += pow((lhs.second)[index], 2);		  //分母(向量的长度)，即(x1^2 + x2^2 + x3^2... + xn^2)
			rhsVectorLength += pow((rhs.second)[index], 2);
			baseVectorLength += pow(base_[index],2);
		}

		if(lhsCrossProduct / (sqrt(lhsVectorLength) * sqrt(baseVectorLength)) 
				< rhsCrossProduct / (sqrt(rhsVectorLength) * sqrt(baseVectorLength)))//按比值从大到小排序
			return false;	
		else
			return true;	
	}	
		
	vector<double> base_;
};

WordQuery::WordQuery(Configuration& conf)
: conf_(conf)
{
	loadLibrary();
}

void WordQuery::loadLibrary()
{
	auto & confMap = conf_.getConfMap();		
	string pagelib_path = confMap["webpage"];
	string offset_path = confMap["offset"];
	
	cout << "----loading library start----" << endl;

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
		
		string doc;//存放一篇网页
		doc.resize(docLen,' ');//初始化docLen长度的空字符串
		inPagelib.seekg(docOffset, inPagelib.beg);//把当前的流位置定位到离流开始位置offset处
		inPagelib.read(&(*doc.begin()), docLen);//从流中读取docLen个字符,写入字符串doc

		WebPage page(doc,conf_);//对文档doc进行处理，方便提取docid,url等

		pageLib_.insert({docID,page});
		offsetLib_.insert({docID,{docOffset,docLen}});
	}

	inPagelib.close();
	inOffset.close();

	//读取倒排索引表
	string invert_index_path = confMap["invert_index_table"];
	ifstream inInvertIndex(invert_index_path);
	if(!inInvertIndex){
		cout << "open invert_index_table error!" << endl;
		return;
	}
	string word;
	double weight;
	while(getline(inInvertIndex,line)){
		istringstream iss(line);
		iss >> word;
		set<pair<int,double>> postItem;//倒排索引项
		while(iss >> docID >> weight){
			postItem.insert({docID,weight});
		}
		invertIndexTable_.insert({word,postItem});
	}
	
	inInvertIndex.close();

	//读取停用词
	string stop_word_path = confMap["stop_word"];
	ifstream ifsStopWords(stop_word_path);
	if(!ifsStopWords){
		cout << "open stop_words file error!" << endl;
		return;
	}
	string stop_word;
	while(getline(ifsStopWords,stop_word)){
		stopwords_.insert(stop_word);
	}

	cout << "----loading library done----" << endl;
}

string WordQuery::doQuery(const string& queryStr)
{
	cout << "query sentence from client: " << queryStr << endl;

	vector<string> queryWordsVec;//存放查询语句的分词结果
	if(queryStr.size() > 0){
		jieba_.cut(queryStr,queryWordsVec);
	}
#if 0
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		cout << *it << "/" ;
	}
	cout << endl;
#endif
	//过滤停用词
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)	
	{
		if(stopwords_.find(*it) != stopwords_.end()){//如果是停用词，则从queryWordsVec中删去
			queryWordsVec.erase(it);
		}
		cout << "当前queryWordsVec.size = " << queryWordsVec.size() << endl;
	}
#if 0
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		cout << *it << "/" ;
	}
	cout << endl;
	//此种情况考虑到，当客户端的查询输入为无效查询词时(即经停用词过滤后queryWordsVec为空的情况)，避免出现崩溃
	if(queryWordsVec.size() == 0)
	{
		cout << "客户端输入无效查询" << endl;
		return noAnswer();
	}
#endif

	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		cout << "key word: " << *it << endl;
		auto retSet = invertIndexTable_.find(*it);
		//但凡有一个关键字在倒排索引表中找不到的,那么认为查询失败,返回noAnswer()的json字符串
		if(retSet == invertIndexTable_.end()){
			cout << "cannot find the key word: " << *it << endl;
			return noAnswer();
		}
	}
	cout << "--------" << endl;;

	//计算查询语句中每个关键字的权重
	vector<double> weightVec = getWeightOfQuery(queryWordsVec);
	Compare cmp(weightVec);//根据基向量初始化比较类
	
	//执行查询，关键！
	//resultVec是传入传出参数,存放着包含所有查询关键字的文档的docID以及对应关键字在该文档中的权重
	vector<pair<int,vector<double>>> resultVec;

	if(executeQuery(queryWordsVec,resultVec)){
		stable_sort(resultVec.begin(),resultVec.end(),cmp);//根据比较规则确定客户端显示的顺序	
		vector<int> docIDVec;
		for(auto it=resultVec.begin(); it!=resultVec.end(); ++it)
		{	
			docIDVec.push_back(it->first);
		}
		return createJson(docIDVec,queryWordsVec);//根据docIDVec去pageLib_中取出文章，根据queryWordsVec自动生成summary
	
	}else{
		return noAnswer();
	}

}

//最关键的函数
bool WordQuery::executeQuery(const vector<string>& queryWordsVec, vector<pair<int,vector<double>>>& resultVec)
{
	cout << "----executing query----" << endl;	
	if(queryWordsVec.size() == 0){//查询语句为空时
		cout << "empty string not find" << endl;
		return false;
	}
	typedef set<pair<int,double>>::iterator setIter;//关键字对应的倒排索引表迭代器
	vector<pair<setIter,int>> iterVec;//保存需要求交集的迭代器,此处int表示已经遍历的迭代器的个数,当这个值等于minIterNum时,说明求交集的过程结束

	int minIterNum = 0x7FFFFFFF;//
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		int sz = invertIndexTable_[*it].size();//关键词对应的倒排索引项个数
		if(sz == 0)//如果为0,说明该关键词找不到,直接返回false
			return false;

		if(sz < minIterNum)//更新
			minIterNum = sz;
		
		iterVec.push_back({invertIndexTable_[*it].begin(),0});//
	}

	//求交集的过程
	bool isExited = false;
	while(!isExited)
	{
		size_t idx = 0;	
	
		for(; idx != iterVec.size()-1; ++idx){
			//比较当前各关键字对应的迭代器所指向的docid,如果docid不同,break
			if((iterVec[idx].first)->first != (iterVec[idx+1].first)->first)
				break;
		}
		
		if(idx == iterVec.size()-1){//说明了什么?说明当前各个关键字对应的迭代器所指向的docid相同,正是我们的所求
			vector<double> weightVec;
			int docID = iterVec[0].first->first;//因为此时iterVec里的迭代器指向的docID都是一样的,所以取第一个(即下标为0)
			//对每一个关键词对应的迭代器都右移一位
			for(idx = 0; idx != iterVec.size(); ++idx)
			{
				weightVec.push_back(iterVec[idx].first->second);
				++(iterVec[idx].first);
				++(iterVec[idx].second);
				if(iterVec[idx].second == minIterNum)
					isExited = true;
			}
			resultVec.push_back({docID,weightVec});

		}else{
			int minDocId = 0x7FFFFFFF;
			int iterIdx;//保存minDocId的迭代器位置
			for(idx = 0; idx != iterVec.size(); ++idx)
			{
				if(iterVec[idx].first->first < minDocId){
					minDocId = iterVec[idx].first->first;
					iterIdx = idx;
				}
			}
			++(iterVec[iterIdx].first);//docID最小的那个迭代器右移
			++(iterVec[iterIdx].second);//移动的位数
			if(iterVec[iterIdx].second == minIterNum)
				isExited = true;	
		}//else				
	}//while

	return true;
}

vector<double> WordQuery::getWeightOfQuery(vector<string>& queryWordsVec)
{
	//统计查询语句的词频
	map<string,int> queryDict;
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		++queryDict[*it];
	}

	//计算权重
	vector<double> weightVec;//存放查询文档关键字对应的权重
	double weightSum = 0;
	int totalPageNum = offsetLib_.size();
//	cout << "totalPageNum: " << totalPageNum << endl;
	for(auto it = queryWordsVec.begin(); it != queryWordsVec.end(); ++it)
	{
		int df = invertIndexTable_[*it].size();//文档频率,即文档集合中出现该关键词的文章数		
		double idf = log(static_cast<double>(totalPageNum)/(df+1));//逆文档频率
		int tf = queryDict[*it];//词频
		double w = idf * tf;
		weightSum += pow(w,2);//用于归一化处理
		weightVec.push_back(w);
	}
	
	//归一化
	for(auto & w : weightVec)
	{
		w /= weightSum;
	}

	return weightVec;
}

string WordQuery::createJson(vector<int>& docidVec, const vector<string>& queryKeyWords)
{
	Json::Value root;
	Json::Value valueArr;
	int cnt = 0;//记录返回给客户端的文章数
	for(size_t i = 0; i != docidVec.size(); ++i)
	{
		string summary = pageLib_[docidVec[i]].getDocSummary(queryKeyWords);//根据查询关键字自动生成摘要
		string title = pageLib_[docidVec[i]].getDocTitle();
		string url = pageLib_[docidVec[i]].getDocUrl();

		Json::Value value;
		value["title"] = title;
		value["url"] = url;
		value["summary"] = summary;
		valueArr.append(value);
		if(++cnt == 100)//最多返回100篇文章
			break;
	}
	root["files"] = valueArr;
	Json::StyledWriter writer;
	return writer.write(root);
}

string WordQuery::noAnswer()
{
	Json::Value root;
	Json::Value valueArr;
	Json::Value value;
	value["title"] = "no title";
	value["url"] = "no url";
	value["summary"] = "no summary";
	valueArr.append(value);
	root["files"] = valueArr;
	Json::StyledWriter writer;
	return writer.write(root);
}

