#ifndef TEMPLATE
#define TEMPLATE

#include "common.h"

int static_value(int n); 
int value(int n=9);  //带默认值参数的函数
int p_hanshu();


//主函数传递指针， 子函数修改值
int change(char *p);

#endif