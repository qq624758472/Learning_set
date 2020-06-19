#include "template.h"

int (*p)(int n);  //不是定义， 这是一个全局变量。
//求n的阶乘。
int value(int n)
{
    for(int i=1; i<=n; i++)
    {
        cout << i <<"的阶乘是:" <<  static_value(i) << endl;
    }
	return 0;
}


int static_value(int n)
{
    static int a = 1;
    a = n * a;
    return a;
}


//指向函数的指针。
int p_hanshu()
{
    p = value;
    p(5);
	return 0;
}