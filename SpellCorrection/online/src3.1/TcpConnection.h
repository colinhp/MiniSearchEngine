 ///
 /// @file    TcpConnection.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 20:22:47
 ///

#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__ 

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <string>
#include <memory>
#include <functional>
using std::string;
using std::shared_ptr;
using std::function;
using std::enable_shared_from_this;
namespace kk
{

class EpollPoller;

class TcpConnection;
typedef shared_ptr<TcpConnection> TcpConnectionPtr; 

class TcpConnection
: Noncopyable
, public enable_shared_from_this<TcpConnection> 
{
public:
	typedef function<void(const TcpConnectionPtr&)> TcpConnectionCallback;
	TcpConnection(int sockfd,EpollPoller* ploop);
	~TcpConnection();

	string recv();
	void send(const string& msg);
	void sendInLoop(const string& msg);
	void shutdown();

	string toString();

	//提供给用户使用,可随意注册回调函数
	void setConnectionCallback(TcpConnectionCallback cb);
	void setMessageCallback(TcpConnectionCallback cb);
	void setCloseCallback(TcpConnectionCallback cb);
	
	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();

private:
	const InetAddress localAddr_;
	const InetAddress peerAddr_;
	Socket socket_;
	SocketIO socketIO_;
	bool isShutdownWrite_;
	EpollPoller* pLoop_;

	TcpConnectionCallback onConnectionCb_;
	TcpConnectionCallback onMessageCb_;
	TcpConnectionCallback onCloseCb_;
};

}//end of namespace kk
#endif
