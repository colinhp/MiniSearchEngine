 ///
 /// @file    Noncopyable.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-22 13:23:20
 ///
 
#ifndef __NONCOPYCABLE_H__
#define __NONCOPYCABLE_H__ 

namespace kk
{
class Noncopyable
{
public:
	Noncopyable(){}
	~Noncopyable(){}
private:
	Noncopyable& operator=(const Noncopyable&);
	Noncopyable(const Noncopyable&);
};

}//end of namespace kk

#endif
