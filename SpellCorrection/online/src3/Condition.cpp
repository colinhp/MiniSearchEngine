 ///
 /// @file    condition.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-16 13:03:57
 ///

#include "Condition.h"
#include "MutexLock.h"

#include <iostream>
using std::cout;
using std::endl;
using namespace kk;

Condition::Condition(MutexLock & mutex)
:_mutex(mutex)
{ pthread_cond_init(&_cond,NULL); }

Condition::~Condition()
{ pthread_cond_destroy(&_cond); }

void Condition::wait()
{ pthread_cond_wait(&_cond,_mutex.getMutecLockPtr()); }

void Condition::notify()
{ pthread_cond_signal(&_cond); }

void Condition::notifyAll()
{ pthread_cond_broadcast(&_cond); }
