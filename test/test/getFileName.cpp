 ///
 /// @file    getFileName.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-31 20:57:58
 ///
 
#include <dirent.h>
#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	DIR* pDir =  opendir("rss");
	if(pDir == NULL){
		cout << "opendir error!" << endl;
		return -1;
	}

	struct dirent* pDirInfo;

	while((pDirInfo = readdir(pDir)) != NULL){
		printf("%s\n",pDirInfo->d_name);
	}
	return 0;
}

