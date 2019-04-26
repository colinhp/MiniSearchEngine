 ///
 /// @file    condition.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 12:56:03
 ///
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace kk
{
	class MutexLock;//前向声明

	class Condition
	:Noncopyable
	{
	public:
		Condition(MutexLock & mutex);
		~Condition();

		void wait();
		void notify();
		void notifyAll();
	private:
		pthread_cond_t _cond;
		MutexLock & _mutex;
	};
}
#endif
