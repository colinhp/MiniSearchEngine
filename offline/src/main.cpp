 ///
 /// @file    main.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 21:24:50
 ///
 
#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include "../include/RssReader.h"
#include "../include/WebPageLib.h"
#include "../include/RemoveDuplicatePage.h"
#include "../include/WordSegmentation.h"
#include "../include/PagelibProcesser.h"

#include <string>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using namespace kk;

int main()
{
	//读取配置文件信息
	string conf_path("/home/kkmjy/C++/RssMiniSearchEngine/offline/conf/conf.txt");
	Configuration* pConf = Configuration::getInstance(conf_path);
	pConf->debug();

	//获取所有xml文件的路径
	DirScanner dirscanner(*pConf);
	dirscanner.debug();
	
	RemoveDuplicatePage* pPageRemover =	RemoveDuplicatePage::getInstance();

	//创建网页库(同时进行去重)
	WebPageLib pagelib(*pConf,dirscanner,*pPageRemover);
	pagelib.createWebPageLib();
	pagelib.storeWebPageLib();
#if 1	
	//建立倒排索引
	PagelibProcesser processer(*pConf);
	processer.doProcess();	
#endif
	return 0;
}
