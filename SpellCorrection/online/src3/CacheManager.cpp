 ///
 /// @file    CacheManager.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-27 10:43:57
 ///
#include "CacheManager.h" 
#include "Cache.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;
using namespace kk;

//定义静态数据成员
map<pthread_t,Cache> CacheManager::cacheMap_;

//初始化函数
void CacheManager::initCache(pthread_t pid, Cache& cache)
{
	Cache currcache(cache);
	cacheMap_.insert({pid,currcache});	
	cout<<"initialize (pid:"<<pid<<" -- cache)"<<endl;
}

Cache& CacheManager::getCache(pthread_t pid)
{
	return cacheMap_[pid];
}

void CacheManager::updateAllCache(const string& cachepath)
{
	auto iter = cacheMap_.begin();

	//1.选定cacheMap中的第一个线程
	Cache cache1 = iter->second; 

	++iter;
	cout<<"----把所有线程的内存cache都写入到同一个cache(记为cache1)中----"<<endl;
	//2.每个一段时间,把所有线程的cache均写入到cache1中
	for( ; iter != cacheMap_.end(); ++iter){
		cache1.update(iter->second);			
	}
	cout<<"----写入cache1,完毕----"<<endl;	
	//3.把cache1写回其他线程的缓存
	iter = cacheMap_.begin();
	++iter;
	cout<<"----把cache1的内容写回其余所有线程----"<<endl;
	for( ; iter != cacheMap_.end(); ++iter){
		iter->second.update(cache1);
	}
	cout<<"----写入其余线程,完毕----"<<endl;
	cout<<"----所有线程cache内容一致----"<<endl;
	for(iter = cacheMap_.begin(); iter != cacheMap_.end(); ++iter){
		cout<<"pid:"<<iter->first<<endl;
		iter->second.show();
		cout<<"=============="<<endl;
	}


	cout<<"----最后把内存cache的内容写回磁盘cache----"<<endl;
	//4.还要把cache1写回磁盘cache
	cache1.writeToFile(cachepath);
}
