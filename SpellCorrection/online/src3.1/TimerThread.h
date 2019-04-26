 ///
 /// @file    TimerThread.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-21 14:15:28
 ///
#ifndef __TIMETHREAD_H__
#define __TIMETHREAD_H__

#include "Timer.h"
#include "Thread.h"

#include <functional>

namespace kk
{
class TimerThread
{
public:
	typedef std::function<void()> TimerCallback;
	TimerThread(TimerCallback cb,int initialTime,int intervalTime);

	void start();
	void stop();
private:
	Timer timer_;
	Thread thread_;
};

}//end of namesapce kk

#endif
