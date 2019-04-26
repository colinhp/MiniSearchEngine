 ///
 /// @file    main.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 21:34:43
 ///

#include "../include/Server.h"
#include "../include/WordQuery.h"

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

using namespace kk;

int main()
{
	string confPath("/home/kkmjy/C++/RssMiniSearchEngine/online/conf/conf.txt");
	Configuration conf(confPath);
	conf.degub();

//	WordQuery wordquery(conf);
//	Server server(conf, wordquery);
	Server server(conf);
	server.start();

	return 0;
}
