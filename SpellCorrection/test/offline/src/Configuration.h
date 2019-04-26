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

//配置文件应该写成单例模式
namespace kk
{
class Configuration
{
public:
	static Configuration* getInstance(const string& confPath);
	static map<string,string>& getConfMap();//返回读取的配置文件信息
	static void destroy();//自动销毁
private:
	//构造函数私有化
	Configuration(const string& confPath);//配置文件的绝对路径
private:
	static map<string,string> confMap_;//存储配置文件的信息,如pair(port,9999);
	static Configuration* pInstance_;
};

}//end of namespace kk

#endif 
