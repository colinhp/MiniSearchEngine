 ///
 /// @file    mutexLock.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 12:42:24
 ///
#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace kk
{
	class MutexLock
	:private Noncopyable
	{
	public:
		MutexLock();
		~MutexLock();

		void lock();
		void unlock();

		pthread_mutex_t* getMutecLockPtr();

		bool isLocking() const;
	private:
		pthread_mutex_t _mutex;
		bool _isLocking;
	};
	
	class MutexLockGuard
	{
	public:
		MutexLockGuard(MutexLock & mutex)
		:_mutex(mutex)
		{
			_mutex.lock();
		}

		~MutexLockGuard()
		{
			_mutex.unlock();
		}
	private:
		MutexLock& _mutex;
	};
}

#endif
