 ///
 /// @file    Server.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-11 20:59:23
 ///
 
#include "../include/Server.h"
#include <iostream>
#include <functional>
using std::cout;
using std::endl;

using namespace std::placeholders;
using namespace kk;

Server::Server(Configuration & conf)
: conf_(conf)
, threadpool_(4,10)
, tcpserver_("192.168.13.128",9999)
, wordquery_(conf_)
{
	tcpserver_.setConnectionCallback(std::bind(&Server::onConnection,this,_1));
	tcpserver_.setMessageCallback(std::bind(&Server::onMessage,this,_1));
	tcpserver_.setCloseCallback(std::bind(&Server::onClose,this,_1));
}

void Server::start()
{
	threadpool_.start();
	tcpserver_.start();
}

void Server::doTask(const TcpConnectionPtr& conn, const string& msg)
{
	string retJson = wordquery_.doQuery(msg);
	conn->sendInLoop(retJson);
}

void Server::onConnection(const TcpConnectionPtr& conn)
{
	cout << conn->toString() << " connected!" << endl;	
	conn->send("From server:hello This is server");
}

void Server::onMessage(const TcpConnectionPtr& conn)
{
	//接收客户端的数据
	string s(conn->recv());

	//剔除'\n',截取子串
	size_t pos = s.find('\n');
	s = s.substr(0,pos);
	
	threadpool_.addTask(std::bind(&Server::doTask,this,conn,s));

}

void Server::onClose(const TcpConnectionPtr& conn)
{
	cout<<conn->toString()<<" closed!"<<endl;
}
