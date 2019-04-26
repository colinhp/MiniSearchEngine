 ///
 /// @file    threadpool.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-17 09:27:24
 ///
 
#include "Threadpool.h"
#include "Thread.h"
#include "Cache.h"
#include "CacheManager.h"

#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using namespace kk;

Threadpool::Threadpool(int threadNum,int queSize,Cache& cache, Dictionary& myDict)
:_threadNum(threadNum)
,_queSize(queSize)
,_cache(cache) 
,_mydict(myDict)
,_taskque(_queSize)
,_isExit(false)
{
	_threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{
	if(!_isExit)
	{ close(); }
}

//仅仅是为了在main函数中通过Threadpool的指针获取到myDict而定义的，用在onMessage函数中
Dictionary& Threadpool::getMyDict()
{ return _mydict; }

void Threadpool::start()
{
	for(int i=0;i!=_threadNum;++i)
	{
		shared_ptr<Thread> sptr(new Thread(std::bind(&Threadpool::threadFunc,this)));
		_threads.push_back(sptr);
	}

	pthread_t pid;
	for(auto & elem : _threads)
	{
		elem->start();//
		pid = elem->getCurrentPid();//获取当前子线程的线程id
		CacheManager::initCache(pid,_cache);//用首次获得的cache初始化每一个子线程(static函数)
	}
}

void Threadpool::close()
{
	if(!_isExit)
	{
		while(!_taskque.empty())
		{
			sleep(1);//合理与否
		}
		_isExit = true;	
		
		//当子线程的运行速度快于主线程时，
		//子线程都会阻塞在_taskQue.pop方法之上，
		//此时需要激活_taskQue内部的条件变量
		_taskque.wakeup();

		for(auto & elem : _threads)
		{
			elem->join();//
		}
	}
}

//充当生产者的角色
void Threadpool::addTask(Task && task)
{
	_taskque.push(std::move(task));
}

//从任务队列中取出要执行的任务
Task Threadpool::getTask()
{
	return _taskque.pop();
}

void Threadpool::threadFunc()
{
	while(!_isExit)
	{
		Task task = getTask();
		if(task)
			task();
	}
}
