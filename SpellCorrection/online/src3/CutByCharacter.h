 ///
 /// @file    CutByCharacter.h
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-30 16:28:38
 ///
 
#ifndef __CUTBYCHARACTER_H__
#define __CUTBYCHARACTER_H__ 
#include <iostream>
#include <string>
using std::string;
namespace kk
{
/// 获取一个字节高位开头为1的个数
size_t nBytesCode(const char ch)
{
	if(ch & (1<<7)){
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx){
			if(ch &(1 << (6-idx))){
				++nBytes;
			}else
				break;
		}
		return nBytes;
	}
	return 1;
}

size_t length(const string& word)
{
	size_t len = 0;
	for(size_t i = 0; i != word.size();  ){
		size_t nbyte = nBytesCode(word[i]);
		i += nbyte;
		++len;
	}
	return len;
}

}//end of namespace kk
#endif
