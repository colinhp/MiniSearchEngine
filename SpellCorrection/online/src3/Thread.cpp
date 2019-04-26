 ///
 /// @file    thread.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 13:15:31
 ///
 
#include "Thread.h"

#include "Cache.h"
#include "CacheManager.h"

#include <iostream>
using std::cout;
using std::endl;
using namespace kk;

Thread::Thread(ThreadCallback && cb)
:_pthId(0)
,_isRunning(false)
,_cb(std::move(cb))
{}

Thread::~Thread()
{
	if(_isRunning)
	{
		pthread_detach(_pthId);
		_isRunning = false; 
	}
}

pthread_t Thread::getCurrentPid()
{
//	return pthread_self();//获取线程id
	return _pthId;
}

void Thread::start()
{
	pthread_create(&_pthId,NULL,threadFunc,this);
	_isRunning = true;
}

void Thread::join()
{	
	if(_isRunning)
	{
		pthread_join(_pthId,NULL);
		_isRunning = false;
	}
}

void* Thread::threadFunc(void *argc)
{
	Thread* p = static_cast<Thread*>(argc);
	if(p)
		p->_cb();
	return NULL;
}
