// redis_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "redis_client.h"


/*
	redis学习测试程序。 
	注意事项：
	1.注意c++设置->代码生成中，调试器为多线程（/MTD）,其他工程为默认。
*/

int main()
{
    std::cout << "Hello World!\n"; 
	redis_client test;
	test.init_net();
	getchar();
}
