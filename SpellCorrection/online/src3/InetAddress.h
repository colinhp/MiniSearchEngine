 ///
 /// @file    InetAddress.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 11:48:16
 ///
#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__ 

#include <netinet/in.h>//for struct sockaddr_in

#include <string>
using std::string;

namespace kk
{

class InetAddress
{
public:
	InetAddress(short port);
	InetAddress(const char* ip,short port);
	InetAddress(const struct sockaddr_in & addr);
	string ip() const;
	unsigned short port() const;
	const struct sockaddr_in* getInetAddressPtr() const;

private:
	struct sockaddr_in addr_;
};

}//end of namespace kk
#endif
