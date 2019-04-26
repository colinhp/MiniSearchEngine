 ///
 /// @file    thread.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 13:11:17
 ///
 
#ifndef __THREAD_H__
#define __THREAD_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace kk
{
	class Thread
	:Noncopyable
	{
	public:
		typedef std::function<void()> ThreadCallback;

		Thread(ThreadCallback && cb);
		~Thread();

		void start();
		void join();
	private:
		static void* threadFunc(void*);
	private:
		pthread_t _pthId;
		bool _isRunning;
		ThreadCallback _cb;
	};
}

#endif
