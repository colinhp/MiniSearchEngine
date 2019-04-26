 ///
 /// @file    main.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 21:34:43
 ///
 
#include "Configuration.h"
#include "Dictionary.h"
#include "MyTask.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "Timer.h"
#include "TimerThread.h"
#include "Cache.h"
#include "CacheManager.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <functional>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::ifstream;
using std::istringstream;
using std::set;
using std::map;

using namespace kk;

Threadpool* pThreadpool = NULL;

void onConnection(const TcpConnectionPtr& conn);
void onMessage(const TcpConnectionPtr& conn);
void onClose(const TcpConnectionPtr& conn);

int main()
{
	string confPath("/home/kkmjy/C++/SpellCorrection/online/conf/conf.txt");
	Configuration conf(confPath);
	//获取配置文件的信息
	map<string,string>& confMap = conf.getConfMap();

/*	//测试
	for(auto iter = confMap.begin(); iter != confMap.end(); ++iter){
		cout<<iter->first<<" "<<iter->second<<endl;
	}
*/
	string dict_path = confMap["dict"];
	string index_path = confMap["index"];
	Dictionary* pMyDict = Dictionary::getInstance(dict_path, index_path);//初始化文词典以及索引文件
//	pMyDict->show();//for debug
		
	string cache_path = confMap["cache"];
	Cache cache;
	cache.readFromFile(cache_path);//在开始执行任务前,先初始化一个内存cache	
	cout<<"++++首次从磁盘中读取cache++++"<<endl;
	cache.show();
	cout<<"+++++++++++++++++++++++++++++"<<endl;

	//创建线程池
	Threadpool threadpool(4,10,cache,*pMyDict);
	threadpool.start();

	pThreadpool = &threadpool;//全局变量

	//初始化定时器
	TimerThread timer(std::bind(&CacheManager::updateAllCache,cache_path),1,10);//
	timer.start();

	string ip = confMap["ip"];
	string port = confMap["port"];
	unsigned short realport = stoi(port);//转换成整型 

	TcpServer tcpserver(ip,realport);
	tcpserver.setConnectionCallback(onConnection);
	tcpserver.setMessageCallback(onMessage);
	tcpserver.setCloseCallback(onClose);

	tcpserver.start();

	return 0;
}


void onConnection(const TcpConnectionPtr& conn)
{
	cout<<conn->toString()<<endl;
	conn->send("From server:hello This is server\n");
}

void onMessage(const TcpConnectionPtr& conn)
{
	//接收客户端的数据
	string s(conn->recv());

	//剔除'\n',截取子串
	string data;
	int pos = s.find('\n');
	if(pos >= 0)
		data = s.substr(0, pos);
	else
		data = s;

	MyTask mytask(data, conn, pThreadpool->getMyDict());//封装任务

	pThreadpool->addTask(std::bind(&MyTask::findQuery,mytask));//交给计算线程处理数据
}

void onClose(const TcpConnectionPtr& conn)
{
	cout<<conn->toString()<<" closed!"<<endl;
}
