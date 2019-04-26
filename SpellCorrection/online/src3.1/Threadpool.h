 ///
 /// @file    threadpool.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-17 09:14:46
 ///
 
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include "Cache.h"
#include "Dictionary.h"

#include <vector>
#include <memory>
#include <functional>
using std::vector;
using std::shared_ptr;

namespace kk
{
	typedef std::function<void()> Task;

	class Thread;

	class Threadpool
	{
	public:
		Threadpool(int threadNum, int queSize, Cache& cache, Dictionary& myDict);
		~Threadpool();

		void start();//启动线程池
		void close();//关闭线程池
		void addTask(Task && task );//向队列增加任务(主线程负责)

		Dictionary& getMyDict();

	private:
		void threadFunc();
		Task  getTask();//从任务队列中取出(子线程负责)
	private:
		int _threadNum;//线程池中子线程的数量
		int _queSize;//任务队列可容纳的最大长度
		Cache _cache;
		Dictionary _mydict;
		vector<shared_ptr<Thread>> _threads;//描述子线程的数组
		TaskQueue _taskque;//任务队列
		bool _isExit;//判断线程池是否关闭
	};
}
#endif
