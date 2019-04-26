 ///
 /// @file    CacheManager.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-27 10:36:46
 ///
 
#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "Cache.h"

#include <pthread.h>
#include <map>
using std::map;

namespace kk
{
class CacheManager
{
public:
	static void initCache(pthread_t pid, Cache& cache);
	static Cache& getCache(pthread_t pid);//获取某个线程对应的缓存
	static void updateAllCache(const string& cachepath);//定时更新所有内存cache,并写回磁盘cache

private:
	static map<pthread_t,Cache> cacheMap_;//线程id与缓存的映射
//	static Cache cache_;
};

}//end of namespace 
#endif
