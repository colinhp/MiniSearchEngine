 ///
 /// @file    Configuration.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 19:50:58
 ///
 
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
using std::string;
using std::map;

namespace kk
{
//配置文件,单例类
class Configuration
{
public:
	static Configuration* getInstance(const string& conf_path);
	static map<string,string>& getConfMap();
	static void debug();//for debug
private:
	Configuration(const string& conf_path);
	static void destroy();//自动销毁函数
private:
	static map<string,string> confMap_;
	static Configuration* pInstance_;
};

}//end of namespace kk

#endif
