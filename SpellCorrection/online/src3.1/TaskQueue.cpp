 ///
 /// @file    taskQueue.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 14:31:14
 ///
 
#include "TaskQueue.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace kk;

TaskQueue::TaskQueue(size_t size)
:_taskNum(size)
,_mutex()//?
,_notEmpty(_mutex)//?
,_notFull(_mutex)//?
,_flag(true)
{}

//把任务压入任务队列
void TaskQueue::push(const ElemType & elem)
{
	MutexLockGuard mlg(_mutex);

	while(full())//如果任务队列是满的
		_notFull.wait();
	_que.push(elem);
	_notEmpty.notify();//通知消费者线程?对于_notEmpty的理解?
}

//从任务队列取出任务
TaskQueue::ElemType TaskQueue::pop()
{
	MutexLockGuard mlg(_mutex);

	while(_flag && empty()){
		_notEmpty.wait();
	}
	if(_flag)
	{
		ElemType tmp = _que.front();
		_que.pop();
		_notFull.notify();//
		return tmp;
	}
	else
		return NULL;
}

bool TaskQueue::empty() const
{
	return _que.empty();  
}

bool TaskQueue::full() const
{
	return _que.size() == _taskNum;
}

//?
void TaskQueue::wakeup()
{
	_flag = false;
	_notEmpty.notifyAll();
}
