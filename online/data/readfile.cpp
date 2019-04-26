 ///
 /// @file    readfile.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-12 10:32:15
 ///
 
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::string;

int main()
{
	ifstream inOffset("offset.lib");
	ifstream inPagelib("webpage.lib");

	string line;
	int docID, docOffset, docLen;
	while(getline(inOffset,line)){

		istringstream iss(line);	
		iss >> docID >> docOffset >> docLen;
		
		string doc;//存放一篇网页
		doc.resize(docLen,' ');//初始化docLen长度的空字符串
		inPagelib.seekg(docOffset, inPagelib.beg);//把当前的流位置定位到离流开始位置offset处
		inPagelib.read(&(*doc.begin()), docLen);//从流中读取docLen个字符,写入字符串doc

		cout << "docLen = " << docLen << endl;
		cout << "----[doc]----"<< endl;
		cout << doc << endl;
		cout << "-------------" << endl;
	}
	return 0;
}
