 ///
 /// @file    Timer.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-21 13:29:49
 ///
 
#ifndef __TIMER_H__
#define __TIMER_H__

#include <functional>

namespace kk
{
class Timer
{
public:
	typedef std::function<void()> TimerCallback;
	
	Timer(TimerCallback cb,int initialTime,int intervalTime);
	~Timer();

	void start();//启动定时器
	void stop();//关闭定时器

private:
	int createTimerfd();
	void setTimerfd(int initialTime,int intervalTime);
	void handleRead();//for what?

private:
	TimerCallback cb_;
	int initialTime_;
	int intervalTime_;
	bool isStarted_;
	int fd_;//与定时器关联的文件描述符
};

}//end of namespace kk

#endif
