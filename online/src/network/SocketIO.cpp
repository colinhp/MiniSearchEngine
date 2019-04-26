 ///
 /// @file    SocketIO.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 19:24:12
 ///
 
#include "SocketIO.h"
#include "SocketUtil.h"
#include <iostream>

using namespace kk;

SocketIO::SocketIO(int sockfd)
:sockfd_(sockfd)
{ }

size_t SocketIO::readn(char* buf,size_t count)
{
	size_t nleft = count;
	char* pbuf = buf;
	while(nleft > 0){
		int nread = ::read(sockfd_, pbuf, nleft);	
//		std::cout<<"SocketIO::readn nread = "<<nread<<std::endl;
		if(-1 == nread){
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}else if(0 == nread){
			break;
		}
		pbuf += nread;
		nleft -= nread;
	}

//	std::cout<<"SocketIO::readn (count - nleft) = "<<(count - nleft)<<std::endl;
	return (count - nleft);//返回读取的字节数
}

size_t SocketIO::writen(const char* buf,size_t count)
{
	size_t nleft = count;
	const char* pbuf = buf;
	while(nleft > 0){
		int nwrite = ::write(sockfd_, pbuf, nleft);	
		if(-1 == nwrite){
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}
		pbuf += nwrite;
		nleft -= nwrite;
	}
	return (count - nleft);//返回读取的字节数
}

//从sockfd_中读取count个字节到buf中，且在tcp中的buffer内仍然保留内容
size_t SocketIO::recvpeek(char* buf, size_t count)
{
	int nread;
	do
	{
		nread = ::recv(sockfd_, buf, count, MSG_PEEK);
		std::cout<<"SocketIO::recvpeek nread = "<<nread<<std::endl;
	}while(nread == -1 && errno == EINTR);
	return nread;
}

size_t SocketIO::readline(char* buf,size_t maxlen)
{
	size_t nleft = maxlen - 1;
	char* pbuf = buf;
	size_t  total = 0;
	while(nleft > 0){
		int nread = recvpeek(pbuf, nleft);
		std::cout<<"SocketIO::readline nread = "<<nread<<std::endl;
		if(nread <= 0)
			return nread;

		//检查'\n',如果有,则说明到了换行,直接返回
		for(size_t idx = 0; idx != (size_t)nread; ++idx){
			if(pbuf[idx] == '\n'){
				size_t nsize = idx + 1;
				if(readn(pbuf,nsize) != nsize)
					return EXIT_FAILURE;
				pbuf += nsize;
				total += nsize;
				*pbuf = 0;
				return total;
			}
		}

		if(readn(pbuf,nread) != (size_t)nread)
			return EXIT_FAILURE;
		pbuf += nread;
		total += nread;
		nleft -= nread;
	}//while	
	*pbuf = 0;
	return maxlen-1;
}

