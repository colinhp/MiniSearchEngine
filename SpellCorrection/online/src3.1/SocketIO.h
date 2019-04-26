 ///
 /// @file    SocketIO.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 19:15:15
 ///

#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include <stdio.h>//for size_t

namespace kk
{
class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char* buf,size_t count);
	size_t writen(const char* buf,size_t count);
	size_t readline(char* buf,size_t maxlen);

private:
	size_t recvpeek(char* buf,size_t count);//for what ?
private:
	int sockfd_;
};

}//end of namespace kk

#endif 
