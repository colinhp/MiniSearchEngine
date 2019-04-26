 ///
 /// @file    TcpServer.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-23 20:16:21
 ///

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "InetAddress.h"
#include "Acceptor.h"
#include "EpollPoller.h"

#include <string>
using std::string;

namespace kk
{
class TcpServer
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;

	TcpServer(const string& ip,unsigned short port);
	TcpServer(unsigned short port);

	void setConnectionCallback(TcpServerCallback cb);
	void setMessageCallback(TcpServerCallback cb);
	void setCloseCallback(TcpServerCallback cb);

	void start();
	void stop();
private:
	Acceptor acceptor_;
	EpollPoller poller_;

	TcpServerCallback connectionCallback_;
	TcpServerCallback messageCallback_;
	TcpServerCallback closeCallback_;
};

}//end of namespace kk

#endif
