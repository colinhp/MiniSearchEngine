 ///
 /// @file    taskQueue.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 13:33:15
 ///
#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <pthread.h>
#include <queue>
#include <functional>
using std::queue;
using std::function;

namespace kk
{
	class TaskQueue
	{
	public:
		typedef function<void()> ElemType;
		TaskQueue(size_t size);
		void push(const ElemType&);
		ElemType pop();
		bool empty() const;
		bool full() const;
		void wakeup();//?
	private:
		size_t _taskNum;//任务队列最多可存放的任务数量
		MutexLock _mutex;
		Condition _notEmpty;
		Condition _notFull;
		queue<ElemType> _que;//队列里存放要执行的任务
		bool _flag;
	};
}
#endif
