/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-03-22 09:41:44
 * @LastEditors: ruog__
 * @LastEditTime: 2023-12-18 10:07:14
 */
#include <iostream>
#include "DuoTai.h"
using namespace std;
/**
 * 当使用类的指针调用成员函数时:
 * 普通函数由指针类型决定.
 * 而虚函数由指针指向的实际类型决定.
 */

int duoTaiTest()
{
    Base *shape;
    Aclass rec(10, 7);
    Bclass tri(10, 5);

    // 存储矩形的地址
    shape = &rec;
    // 调用矩形的求面积函数 area
    shape->area();
    shape->area2();
    shape->test();

    // 存储三角形的地址
    shape = &tri;
    // 调用三角形的求面积函数 area
    shape->area();
    shape->area2();
    shape->test();

    return 0;
}