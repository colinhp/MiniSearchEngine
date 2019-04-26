 ///
 /// @file    Socket.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 13:12:38
 ///
 
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Noncopyable.h"

namespace kk
{
class InetAddress;

class Socket
:Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();
	
	int sockfd() const;
	void shutdownWrite();
	void nonblock();

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);
private:
	int sockfd_;
};

}//end of namespce kk

#endif
