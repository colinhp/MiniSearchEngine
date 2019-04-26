 ///
 /// @file    Acceptor.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 17:18:39
 ///
 
#include "Acceptor.h"
#include "unistd.h"//close()

#define MAX_LISTEN_NUM 10

using namespace kk;

Acceptor::Acceptor(const InetAddress& addr,int listenfd)
: addr_(addr)
, listenSock_(listenfd)
{ }

//服务器端的准备工作
void Acceptor::ready()
{
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

//绑定
void Acceptor::bind()
{
	socklen_t len = sizeof(addr_);
	::bind(listenSock_.sockfd(),
		  (struct sockaddr*)addr_.getInetAddressPtr(),
		  len);	
}

//监听
void Acceptor::listen()
{
	::listen(listenSock_.sockfd(),MAX_LISTEN_NUM);
}

//接收请求
int Acceptor::accept()
{
	int newFd =::accept(listenSock_.sockfd(),NULL,NULL);
	return newFd;  
}

//设置地址重用
void Acceptor::setReuseAddr(bool flag)	
{
	int on = (flag ? 1 : 0);
	if(::setsockopt(listenSock_.sockfd(),
					SOL_SOCKET,
					SO_REUSEADDR,
					&on,
					(socklen_t)(sizeof(on))) == -1)
	{
		perror("setsockopt reuseaddr error");
		::close(listenSock_.sockfd());
		exit(EXIT_FAILURE);
	}
}

//设置端口号重用
void Acceptor::setReusePort(bool flag)
{
	int on = (flag ? 1 : 0);
	if(::setsockopt(listenSock_.sockfd(),
					SOL_SOCKET,
					SO_REUSEPORT,
					&on,
					(socklen_t)(sizeof(on))) == -1)
	{
		perror("setsockopt reuseport error");
		::close(listenSock_.sockfd());
		exit(EXIT_FAILURE);
	}
}

