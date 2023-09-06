/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-07-31 14:22:51
 * @LastEditors: ruog__
 * @LastEditTime: 2023-07-31 14:29:51
 */
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>


#if 0 //这样是报错的。
int main() {
    unsigned int addr;
    int a;

    addr = &a; // 将常量a的地址赋给addr
    a = 99;
    printf("*addr的值为：%d\n", *((int*)addr)); // 打印*addr的值

    a = 55;
    printf("*addr的值为：%d\n", *((int*)addr)); // 打印*addr的值

    return 0;
}
#else
int main()  //这样是正确的
{
    intptr_t addr;
    int a;

    addr = (intptr_t)&a; // 将常量a的地址赋给addr
    a = 99;
    printf("*addr的值为：%d\n", *((int *)addr)); // 打印*addr的值

    a = 55;
    printf("*addr的值为：%d\n", *((int *)addr)); // 打印*addr的值

    return 0;
}
#endif