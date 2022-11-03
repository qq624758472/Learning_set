//  allen
//
//  Created by Allenboy on 2018/1/20.
//  Copyright © 2018年 Allenboy. All rights reserved.
//
//编译能过 可以生成可执行文件  gcc main2.cpp (a.out) 或者 gcc -o test main2.cpp (test) 运动时用 ./a.out(./test)
// gcc version 7.2.0 (Ubuntu 7.2.0-8ubuntu3) 
#include<stdio.h>
int main()
{
    int data1 = 10;
    int data2 = 20;
    int result;
    
    asm (
        "imull %%edx,%%ecx\n\t"
        "movl %%ecx,%%eax\n\t"
        : "=a" (result)
        : "d" (data1), "c" (data2)
        );
    printf("the result is %d\n",result);
    return 0;
}