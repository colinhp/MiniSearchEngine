 ///
 /// @file    EpollPoller.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 22:46:14
 ///
 
#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <functional>

namespace kk
{
class Acceptor;

class EpollPoller
:Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	typedef std::function<void()> Functor;

	EpollPoller(Acceptor& acceptor);
	~EpollPoller();

	void loop();
	void unloop();
	void runInLoop(const Functor& cb);
	void doPendingFunctors();

	void wakeup();
	void handleRead();//

	void setConnectionCallback(EpollCallback cb);
	void setMessageCallback(EpollCallback cb);
	void setCloseCallback(EpollCallback cb);

private:
	void waitEpollfd();
	void handleConnection();
	void handleMessage(int peerfd);
private:
	Acceptor& acceptor_;
	int epollfd_;
	int listenfd_;
	int wakeupfd_;
	bool isLooping_;

	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;//?

	typedef std::vector<struct epoll_event> Eventlist;
	Eventlist eventlist_;//想要检测的事件的注册列表

	typedef std::map<int,TcpConnectionPtr> ConnectionMap;
	ConnectionMap connMap_;

	EpollCallback onConnectionCb_;
	EpollCallback onMessageCb_;
	EpollCallback onCloseCb_;
};

}//end of namespace kk

#endif
