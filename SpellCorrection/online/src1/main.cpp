 ///
 /// @file    main.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 21:34:43
 ///
 
#include "Configuration.h"

#include "MyTask.h"
#include "TcpServer.h"
#include "Threadpool.h"

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

	//创建线程池
	Threadpool threadpool(4,10);
	threadpool.start();

	//
	//
	//
	pThreadpool = &threadpool;//for what?

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
	size_t i;
	for(i = 0; i != s.size(); ++i){
		if(s[i] == '\n')
			break;
	}	
	s = s.substr(0,i);

	MyTask mytask(s,conn);//封装任务
	mytask.init();
	pThreadpool->addTask(std::bind(&MyTask::findQuery,mytask));//交给计算线程处理数据
}

void onClose(const TcpConnectionPtr& conn)
{
	cout<<conn->toString()<<" closed!"<<endl;
}