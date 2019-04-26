 ///
 /// @file    TimerThread.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-21 14:20:23
 ///
 
#include "TimerThread.h"
#include <iostream>
using std::cout;
using std::endl;

using namespace kk;

TimerThread::TimerThread(TimerCallback cb,int initialTime, int intervalTime)
: timer_(cb,initialTime,intervalTime)
, thread_(std::bind(&Timer::start,&timer_)) 
{ }

//在子线程执行定时器
void TimerThread::start()
{
	thread_.start();//创建一个子线程
}

//在主线程关闭定时器
void TimerThread::stop()
{
	timer_.stop();
	thread_.join();
}
