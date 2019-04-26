 ///
 /// @file    Acceptor.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 17:12:15
 ///
#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"

#include <string>
using std::string;

namespace kk
{
class Acceptor
{
public:
	Acceptor(const InetAddress& addr_,int listenfd);
	void ready();
	int accept();
	int sockfd() const
	{ return listenSock_.sockfd(); }

private:
	void setReuseAddr(bool);	
	void setReusePort(bool);
	void bind();
	void listen();

private:
	InetAddress addr_;
	Socket listenSock_;
};

}//end of namespace kk

#endif 
