/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-03-22 08:48:17
 * @LastEditors: ruog__
 * @LastEditTime: 2023-03-23 16:34:07
 */
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



// const char *ptr;
// char const *ptr;  
//上述两个相同， const修饰char ,定义一个指向字符常量的指针，*ptr的值为const，不能修改。
// 定义一个指向字符常量的指针，这里，ptr是一个指向 char* 类型的常量，
// 所以不能用ptr来修改所指向的内容，换句话说，*ptr的值为const，不能修改。
// 但是ptr的声明并不意味着它指向的值实际上就是一个常量，而只是意味着对ptr而言，这个值是常量。
// 还可以通过重新赋值给该指针来修改指针指向的值

// char * const ptr;
// 定义一个指向字符的指针常数，即const指针，实验得知，不能修改ptr指针，但是可以修改该指针指向的内容。

class ConstTest
{
    public:
    const char *ptr1;
    char const *ptr2;
    char * const ptr3 = buff2;

    char buff[4096];
    char buff2[4096];

    ConstTest();

    bool test();
};
#endif