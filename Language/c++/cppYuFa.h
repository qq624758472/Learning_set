#pragma once

#ifndef CPPYUFA
#define CPPYUFA

#include <deque>
#include <string>
#include <assert.h>
using namespace std;

class CppYuFa
{
public:
	//数组初始化
	//初始化数组时，可以省略等号，c++11
	double test1[4]{ 1.2e4, 1.2e4, 1.2e4, 1.2e4 };
	//不在大括号内包含任何东西，将默认设置为0
	float test2[100]{};
	//初始化禁止缩窄操作，例： 不能 将long test3[3] {25, 25, 0.4}    0.4为浮点型。  


	void putDeque(char* que);//入队列
	int frameNumCache = 160;
	std::deque<char*> seqImages;
};


#endif