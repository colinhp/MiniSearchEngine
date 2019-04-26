 ///
 /// @file    Server.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-11 20:47:10
 ///
 
#ifndef __SERVER_H__
#define __SERVER_H__


#include "./network/Threadpool.h"
#include "./network/TcpServer.h"
#include "Configuration.h"
#include "WordQuery.h"

#include <iostream>
using std::cout;
using std::endl;

namespace kk
{
class Server
{
public:
//	Server(Configuration & conf, WordQuery & wordquery);
	Server(Configuration & conf);
	
	void start();

private:
	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn);	
	void onClose(const TcpConnectionPtr& conn);
	void doTask(const TcpConnectionPtr& conn, const string& msg);

private:
	Configuration conf_;
	Threadpool threadpool_;
	TcpServer tcpserver_;
	WordQuery wordquery_;
};

}//end of namespace kk
#endif
