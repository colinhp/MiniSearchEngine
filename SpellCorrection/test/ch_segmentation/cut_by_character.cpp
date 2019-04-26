 ///
 /// @file    create_index.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-29 22:31:47
 ///
 
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

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

void cutByCharacter(string& line)
{
	for(size_t idx = 0; idx != line.size(); ){
		int nbyte = nBytesCode(line[idx]);
		string ch = line.substr(idx, nbyte);
		cout << ch << " ";
		idx += nbyte;
	}	
	cout << endl;
}

int main()
{
	string line = "深圳王道程序员训练营";
	cutByCharacter(line);
	line = "shenzhen";
	cutByCharacter(line);
	return 0;
}
