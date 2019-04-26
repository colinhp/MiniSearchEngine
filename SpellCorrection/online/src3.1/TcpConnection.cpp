 ///
 /// @file    TcpConnection.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 20:33:09
 ///

#include "TcpConnection.h"
#include "EpollPoller.h"
#include <string.h>
#include <iostream>
using namespace kk;

TcpConnection::TcpConnection(int sockfd,EpollPoller* ploop)
: localAddr_(Socket::getLocalAddr(sockfd))
, peerAddr_(Socket::getPeerAddr(sockfd))
, socket_(sockfd)
, socketIO_(sockfd)
, isShutdownWrite_(false)
, pLoop_(ploop)
{ 
	socket_.nonblock();//for what?
}

TcpConnection::~TcpConnection()
{
	if(!isShutdownWrite_){
		isShutdownWrite_ = true;
//		socket_.shutdownWrite();//why not this one?and difference?
		shutdown();
	}
}

//读到buf里
string TcpConnection::recv()
{
	char buf[65536];
	memset(buf,0,sizeof(buf));
	size_t ret = socketIO_.readline(buf,sizeof(buf));
//	std::cout<<"TcpConnection::recv ret = "<<ret<<std::endl;
	if(ret == 0){
		return string();//返回一个空字符
	}else{
		return string(buf);
	}
}

//先写到buf里
void TcpConnection::send(const string& msg)
{
	socketIO_.writen(msg.c_str(),msg.size());
}

//在计算线程里面调用
void TcpConnection::sendInLoop(const string& msg)
{
	pLoop_->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

void TcpConnection::shutdown()
{
	if(!isShutdownWrite_)
		socket_.shutdownWrite();
	isShutdownWrite_ = true;
}

string TcpConnection::toString()
{
	char buf[100];
	snprintf(buf,sizeof(buf),"%s:%d --> %s:%d",
			localAddr_.ip().c_str(),
			localAddr_.port(),
			peerAddr_.ip().c_str(),
			peerAddr_.port());

	return string(buf);
}

void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
	onConnectionCb_ = cb;
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
	onMessageCb_ = cb;
}

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
	onCloseCb_ = cb;
}

void TcpConnection::handleConnectionCallback()
{
	if(onConnectionCb_)
		onConnectionCb_(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
	if(onMessageCb_)
		onMessageCb_(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
	if(onCloseCb_)
		onCloseCb_(shared_from_this());
}



