 ///
 /// @file    SocketUtil.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 13:51:43
 ///
 
#ifndef __SOCKETUTIL_H__
#define __SOCKETUTIL_H__ 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

namespace kk
{

inline int createSocketfd()
{
	int fd = ::socket(AF_INET,SOCK_STREAM,0);
	if(fd == -1){
		perror("socket create error");
	}
	return fd;
}

//将文件描述符fd对应的文件设置为非阻塞的
inline void setNonblock(int fd)
{
	int flag = ::fcntl(fd,F_GETFL,0);
	flag |= O_NONBLOCK;
	::fcntl(fd,F_SETFL,flag);
}

inline int createEpollfd()
{
	int epfd = ::epoll_create1(0);//创建epoll实例
	if(-1 == epfd){
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return epfd;
}


inline int createEventfd()
{
	int evfd = ::eventfd(0,0);
	if(-1 == evfd){
		perror("eventfd create error");
	}
	return evfd;
}

//注册事件
inline void addEpollReadFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
	if(-1 == ret){
		perror("epoll_ctl add error");
		exit(EXIT_FAILURE);
	}
}

//解除事件
inline void delEpollReadFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
	if(-1 == ret){
		perror("epoll_ctl del error");
		exit(EXIT_FAILURE);
	}
}

//预览数据
inline size_t recvpeek(int peerfd, void* buf, size_t len)
{
	int nread;
	do
	{
		nread = ::recv(peerfd, buf, len, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);
	return nread;
}

//通过预览数据，判断conn是否关闭
inline bool isConnectionClosed(int peerfd)
{
	char buf[1024];
	int nread = recvpeek(peerfd, buf, sizeof(buf));
	if(-1 == nread){//连接已经关闭
		perror("recvpeek");
		return true;
	}
	return (0 == nread);
}

}//end of namespace kk

#endif
