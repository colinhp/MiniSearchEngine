 ///
 /// @file    Socket.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 13:19:08
 ///
 
#include "InetAddress.h"
#include "Socket.h"
#include "SocketUtil.h"

using namespace kk;

Socket::Socket(int sockfd)
:sockfd_(sockfd)
{}

Socket::Socket()
:sockfd_(createSocketfd())
{}

Socket::~Socket()
{
	::close(sockfd_);
}

void Socket::shutdownWrite()
{
	if(::shutdown(sockfd_,SHUT_WR) == -1){
		perror("shutdown write error!");
	}	
}

int Socket::sockfd() const
{
	return sockfd_;
}

void Socket::nonblock()
{
	setNonblock(sockfd_);	
}

InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getsockname(sockfd,(struct sockaddr*)&addr,&len) == -1){
		perror("getsockname error");
	}
	return InetAddress(addr);
}

InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getpeername(sockfd,(struct sockaddr*)&addr,&len) == -1){
		perror("getpeername error");
	}
	return InetAddress(addr);
}

