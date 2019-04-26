 ///
 /// @file    TcpServer.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 09:35:14
 ///
 
#include "TcpServer.h"
#include "InetAddress.h"
#include "SocketUtil.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace kk;

TcpServer::TcpServer(const string& ip,unsigned short port)
: acceptor_(InetAddress(ip.c_str(),port),createSocketfd())
, poller_(acceptor_) 
{ }

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{ connectionCallback_ = cb; }

void TcpServer::setMessageCallback(TcpServerCallback cb)
{ messageCallback_ = cb; }

void TcpServer::setCloseCallback(TcpServerCallback cb)
{ closeCallback_ = cb; }

void TcpServer::start()
{
	acceptor_.ready();
	poller_.setConnectionCallback(connectionCallback_);
	poller_.setMessageCallback(messageCallback_);
	poller_.setCloseCallback(closeCallback_);
	poller_.loop();
}

void TcpServer::stop()
{
	poller_.unloop();
}

