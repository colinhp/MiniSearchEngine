 ///
 /// @file    EpollPoller.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-23 00:09:02
 ///
 
#include "EpollPoller.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#include <assert.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace kk;

EpollPoller::EpollPoller(Acceptor& acceptor)
: acceptor_(acceptor)
, epollfd_(createEpollfd())
, listenfd_(acceptor_.sockfd())
, wakeupfd_(createEventfd())
, isLooping_(false)
, eventlist_(1024)//可以直接这样么？
{
	addEpollReadFd(epollfd_, listenfd_);
	addEpollReadFd(epollfd_, wakeupfd_);
}

EpollPoller::~EpollPoller()
{
	::close(epollfd_);
}

void EpollPoller::loop()
{
	isLooping_ = true;
	while(isLooping_){
		waitEpollfd();
	}
}	

void EpollPoller::unloop()
{
	if(isLooping_){
		isLooping_ = false;
	}
}

void EpollPoller::runInLoop(const Functor& cb)
{
	{
		MutexLockGuard mlg(mutex_);
		pendingFunctors_.push_back(cb);
	}
	wakeup();
}

//执行回调函数,这里的回调函数是指?
void EpollPoller::doPendingFunctors()
{
	std::vector<Functor> functors;
	{
		MutexLockGuard mlg(mutex_);
		functors.swap(pendingFunctors_);//???
	}
	for(size_t i =0; i < functors.size(); ++i){
		functors[i]();//typedef function<void()> Functor
	}
}

//唤醒wakeupfd_(即eventfd_)
void EpollPoller::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(wakeupfd_, &one, sizeof(one));
	cout<<"n = "<<n<<" sizeof(one) = "<<sizeof(one)<<endl;
	if(n != sizeof(one)){
		perror("EpollPoller::wakeup error!");
	}
}

void EpollPoller::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(wakeupfd_, &one, sizeof(one));
	if(n != sizeof(one)){
		perror("EpollPoller::read error!");
	}
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	onConnectionCb_ = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	onMessageCb_ = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	onCloseCb_ = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	//为什么用do-while处理?
	do
	{
		nready = epoll_wait(epollfd_,
							&(*eventlist_.begin()),
							eventlist_.size(),
							5000);
	}while(-1 == nready && errno == EINTR);

	if(-1 == nready){
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}else if(nready == 0){
		cout<<"epoll_wait timeout!"<<endl;
	}else{
		//如果激活的事件数量等于epoll的容量，则扩容操作
		if(nready == static_cast<int>(eventlist_.size())){
			eventlist_.resize(eventlist_.size()*2);	
		}
		//遍历每一个激活的文件描述符
		for(int i=0; i!=nready; ++i){
			//如果接收到新的连接请求
			if(eventlist_[i].data.fd == listenfd_){
				if(eventlist_[i].events &  EPOLLIN){//事件！
					handleConnection();//建立连接
				}
			}
			else if(eventlist_[i].data.fd == wakeupfd_){//如果被激活的是事件通知描述符
				cout<<"wakeupfd light"<<endl;
				if(eventlist_[i].events & EPOLLIN){
					handleRead();//则通知
					doPendingFunctors();//去执行每个回调函数
				}	
			}
			//如果响应的是其他已经注册的事件,则向对端发送消息
			else{
				if(eventlist_[i].events & EPOLLIN){
					handleMessage(eventlist_[i].data.fd);	
				}
			}
		}//end of for

	}//end of else
}

//
void EpollPoller::handleConnection()
{
	int peerfd = acceptor_.accept();
	addEpollReadFd(epollfd_, peerfd);//事件注册
	
	TcpConnectionPtr conn(new TcpConnection(peerfd, this));

	conn->setConnectionCallback(onConnectionCb_);
	conn->setMessageCallback(onMessageCb_);
	conn->setCloseCallback(onCloseCb_);

	//for what?
	auto ret = connMap_.insert(std::make_pair(peerfd,conn));
	assert(ret.second == true);
	(void)ret;//for what?
	
	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	auto iter = connMap_.find(peerfd);
	assert(iter != connMap_.end());
	if(isClosed){
		iter->second->handleCloseCallback();
		delEpollReadFd(epollfd_, peerfd);
		connMap_.erase(iter);
	}else{
		iter->second->handleMessageCallback();
	}
}

