 ///
 /// @file    getJSON.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-11 14:35:07
 ///
 
#include <json/json.h>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;


//{"files":[{"summary":"xxx","title":"xxxx"},{},{},{},{}]}


int main()
{
	Json::Value root;
	Json::Value elemArr;
	
	for(int i = 1;i <= 5;i++)
	{
		Json::Value elem;
		elem["summary"] = "this is a summary test...";
		elem["title"] = string(i,'A'); 
		elem["url"] = "url...";
		elemArr.append(elem);
	}
	root["files"] = elemArr;
	Json::StyledWriter writer;
	string str = writer.write(root);

//********************************************//
	
	Json::Value root2;
	Json::Reader reader;
	Json::Value elemArr2;
	if(!reader.parse(str,root2,false)){//解析string型字符串
		return -1;
	}
	int	size = root2["files"].size();
	cout << "size = " << size << endl;
	elemArr2 = root2["files"];
	string summary;
	string title;
	string url;
	for(int i = 0; i < size; i++)
	{	
		summary = elemArr2[i]["summary"].asString();
		title = elemArr2[i]["title"].asString();
		url = elemArr2[i]["url"].asString();
		cout << "summary: " << summary << "\t title: " << title << "\t url: " << url << endl;
	}

	return 0;
}
