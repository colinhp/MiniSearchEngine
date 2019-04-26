 ///
 /// @file    mutexLock.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 12:48:37
 ///
 
#include "MutexLock.h"
using namespace kk;

MutexLock::MutexLock()
:_isLocking(false)
{ pthread_mutex_init(&_mutex,NULL); }

MutexLock::~MutexLock()
{ pthread_mutex_destroy(&_mutex); }

void MutexLock::lock()
{
	_isLocking = true;
	pthread_mutex_lock(&_mutex);
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
	_isLocking = false;
}

pthread_mutex_t* MutexLock::getMutecLockPtr() 
{ return &_mutex; }

bool MutexLock::isLocking() const
{ return _isLocking; }


