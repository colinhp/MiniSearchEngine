 ///
 /// @file    EditDistance.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-08-30 20:17:27
 ///
 
#include "CutByCharacter.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

int minOfThree(int a, int b, int c)
{
	return (a < b) ? (a < c ? a : c) : (b < c ? b : c);
}

//计算字符串共有几个字符(注意不是字节！)
int length(const string& word)
{
	int len = 0;
	for(size_t i = 0; i != word.size();  ){
		size_t nbyte = nBytesCode(word[i]);
		i += nbyte;
		++len;
	}
	return len;
}

//求最短编辑距离
int editDistance(const string& lhs, const string& rhs)
{
//	size_t lenRhs = rhs.size();//处理中文就不适合了
//	size_t lenLhs = lhs.size();

	size_t lenRhs = length(rhs);
	size_t lenLhs = length(lhs);
		
	int editDist[lenLhs+1][lenRhs+1];
	
	for(size_t i = 0; i<=lenLhs; ++i){
		editDist[i][0] = i;
	}

	for(size_t i = 0; i<=lenRhs; ++i){
		editDist[0][i] = i;
	}

	string subLhs, subRhs;

	for(size_t i = 1, subi = 0; i <= lenLhs; ++i){
		size_t nbyte = nBytesCode(lhs[subi]);
		subLhs = lhs.substr(subi,nbyte);//截取子串
		subi += nbyte;

		for(size_t j = 1, subj = 0; j <= lenRhs; ++j){
			nbyte = nBytesCode(rhs[subj]);
			subRhs = rhs.substr(subj,nbyte);//截取子串
			subj += nbyte;

			if(subLhs == subRhs){
				editDist[i][j] = editDist[i-1][j-1];
			}else{
				editDist[i][j] = minOfThree(editDist[i-1][j]+1,
											editDist[i][j-1]+1,
											editDist[i-1][j-1]+1);
			}
		}
	}//for
	return editDist[lenLhs][lenRhs];
}

int main()
{
	string word1("this\n"), word2("ths");
//	cout<<editDistance(word1,word2)<<endl;
	cout << length(word1) << endl;

	word1 = "中国人\n", word2 = "中国人";
	string  word3 = "中国abc人";
//	cout<<editDistance(word1,word2)<<endl;
	cout << length(word1) << endl;
	cout << length(word2) << endl;
	cout << length(word3) << endl;

	return 0;
}
