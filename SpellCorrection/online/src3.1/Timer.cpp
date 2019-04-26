 ///
 /// @file    Timer.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-21 13:35:56
 ///
 
#include "Timer.h" 

#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;


using namespace kk;

Timer::Timer(TimerCallback cb,int initialTime,int intervalTime)
: cb_(cb) 
, initialTime_(initialTime)
, intervalTime_(intervalTime)
, isStarted_(false)
, fd_(createTimerfd())
{}

Timer::~Timer()
{
	if(isStarted_)
		stop();
}

void Timer::start()
{
	struct pollfd pfd;//poll
	pfd.fd = fd_;
	pfd.events = POLLIN;
	
	isStarted_ = true;
	setTimerfd(initialTime_, intervalTime_);//开启定时器
	
	while(isStarted_)
	{
		int nready = ::poll(&pfd,1,/*5000*/-1);
		if(nready == -1 && errno == EINTR)
			continue;
		else if(nready == -1){
			cout<<"poll error"<<endl;
			return;
		}
		else if(nready == 0){
			cout<<"poll timeout!"<<endl;//超时
		}
		else{
			if((pfd.fd == fd_)  && (pfd.revents & POLLIN )){
				handleRead();//处理定时器的操作
				if(cb_)
					cb_();
			}	
		}

	}//while

}

void Timer::stop()
{
	setTimerfd(0,0);//关闭定时器
	if(isStarted_){
		isStarted_ = false;
	}
}

int Timer::createTimerfd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME,0);
	if(fd == -1){
		perror("timerfd_create error");
	}
	return fd;
}

void Timer::setTimerfd(int initialTime,int intervalTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initialTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = intervalTime;
	value.it_interval.tv_nsec = 0;

	int ret = ::timerfd_settime(fd_,0,&value,NULL);
	if(ret == -1){
		perror("timerfd_settime error");
	}
}

//for what?
void Timer::handleRead()
{
	uint64_t count;//必须是uint64_t
//	int count;//error!why?因为,见manual man timerfd_create的说明
	int ret = ::read(fd_,&count,sizeof(count));
//	cout<<"sizeof(count) = "<<sizeof(count)<<" ret = "<<ret<<endl;
	if(ret != sizeof(count)){
		perror("read error");
	}
}

