 ///
 /// @file    InetAddress.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 12:07:36
 ///
 
#include "InetAddress.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

using namespace kk;

InetAddress::InetAddress(short port)
{
	memset(&addr_,0,sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = INADDR_ANY; 
}

InetAddress::InetAddress(const char* ip, short port)
{
	memset(&addr_,0,sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);//主机字节序-->网络字节序
	addr_.sin_addr.s_addr = inet_addr(ip);//字符串形式-->二进制数
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
:addr_(addr)
{}

const struct sockaddr_in* InetAddress::getInetAddressPtr() const
{
	return &addr_;	
}

string InetAddress::ip() const
{
	return string(inet_ntoa(addr_.sin_addr));//二进制数-->字符串
}

unsigned short InetAddress::port() const
{
	return ntohs(addr_.sin_port);
}

