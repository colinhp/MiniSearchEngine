 ///
 /// @file    DirScanner.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 20:33:09
 ///
 
#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include "Configuration.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace kk
{
class Configuration;

class DirScanner
{
public:
	DirScanner(const Configuration& conf);
	vector<string>& getFilePath();//对外提供的接口
	void debug();//for debug
private:
	void assembleFilePath();//拼接路径
private:
	Configuration conf_;
	vector<string> filePath_;//存放.xml文件的路径	
};

}//end of namespace kk

#endif
