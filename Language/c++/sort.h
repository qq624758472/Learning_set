/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-04-04 13:34:12
 * @LastEditors: ruog__
 * @LastEditTime: 2023-04-04 13:34:12
 */

#ifndef SORT
#define SORT

#include "common.h"

class Sort
{
    // protected:
public:
    int width, height;
    int a, b;

public:
    Sort(int a = 0, int b = 0) : width(a), height(b) {}

    int area()
    {
        cout << "Parent class area:" << endl;
        return 0;
    }

    int division();
};

#endif