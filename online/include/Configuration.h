 ///
 /// @file    Configuration.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-24 21:42:46
 ///
 
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__ 

#include <string>
#include <map>
using std::string;
using std::map;

namespace kk
{
class Configuration
{
public:
	Configuration(const string& confPath);//配置文件的绝对路径
	Configuration(){}

	map<string,string>& getConfMap();//返回读取的配置文件信息

	void degub();
private:
	string path_;
	map<string,string> confMap_;//存储配置文件的信息,如pair(port,9999);
};

}//end of namespace kk

#endif 
